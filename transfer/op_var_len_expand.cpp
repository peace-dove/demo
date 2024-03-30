/**
 * Copyright 2022 AntGroup CO., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

//
// Created by wt on 18-8-30.
// bxj on 24-3-30
//

#include "cypher/execution_plan/ops/op_var_len_expand.h"
#include <memory>
#include "cypher_types.h"

namespace cypher {

DfsState::DfsState(RTContext *ctx, lgraph::VertexId id, int level, cypher::Relationship *relp,
                   ExpandTowards expand_direction, bool needNext, bool isMaxHop)
    : currentNodeId(id), level(level), count(1), needNext(needNext) {
    auto &types = relp->Types();
    auto iter_type = lgraph::EIter::NA;
    switch (expand_direction) {
    case ExpandTowards::FORWARD:
        iter_type = types.empty() ? lgraph::EIter::OUT_EDGE : lgraph::EIter::TYPE_OUT_EDGE;
        break;
    case ExpandTowards::REVERSED:
        iter_type = types.empty() ? lgraph::EIter::IN_EDGE : lgraph::EIter::TYPE_IN_EDGE;
        break;
    case ExpandTowards::BIDIRECTIONAL:
        iter_type = types.empty() ? lgraph::EIter::BI_EDGE : lgraph::EIter::BI_TYPE_EDGE;
        break;
    }
    if (!isMaxHop) {
        // if max hop, do not init eiter
        (relp->ItsRef()[level]).Initialize(ctx->txn_->GetTxn().get(), iter_type, id, types);
        currentEit = &(relp->ItsRef()[level]);
    }
}

bool VarLenExpand::PerNodeLimit(RTContext *ctx, size_t count) {
    return !ctx->per_node_limit_.has_value() || count <= ctx->per_node_limit_.value();
}

bool VarLenExpand::NextWithFilter(RTContext *ctx) {
    while (!stack.empty()) {
        if (needPop) {
            relp_->path_.PopBack();
            needPop = false;
        }
        auto &currentState = stack.back();
        auto currentNodeId = currentState.currentNodeId;
        auto &currentEit = currentState.currentEit;
        auto currentLevel = currentState.level;

        // TODO the part of count, needs check
        auto &currentCount = currentState.count;
        if (!PerNodeLimit(ctx, currentCount)) {
            stack.pop_back();
            if (relp_->path_.Length() != 0) {
                relp_->path_.PopBack();
            }
            continue;
        }

        auto &needNext = currentState.needNext;

        // if currentNodeId's needNext = true, currentEit.next
        // then set needNext = false
        if (needNext) {
            currentEit->Next();
            currentCount++;
            needNext = false;
        }

        if (currentLevel == max_hop_) {
            // check path unique
            if (ctx->path_unique_ && relp_->path_.Length() != 0) {
                CYPHER_THROW_ASSERT(pattern_graph_->VisitedEdges().Erase(
                    relp_->path_.GetNthEdgeWithTid(relp_->path_.Length() - 1)));
            }
            stack.pop_back();
            neighbor_->PushVid(currentNodeId);

            // check label
            if (!neighbor_->Label().empty() && neighbor_->IsValidAfterMaterialize(ctx) &&
                neighbor_->ItRef()->GetLabel() != neighbor_->Label()) {
                if (relp_->path_.Length() != 0) {
                    relp_->path_.PopBack();
                }
                continue;
            }

            if (relp_->path_.Length() != 0) {
                needPop = true;
            }

            return true;
        }

        if (currentEit->IsValid()) {
            // check path unique
            if (ctx->path_unique_ && pattern_graph_->VisitedEdges().Contains(*currentEit)) {
                currentEit->Next();
                currentCount++;
                continue;
            } else if (ctx->path_unique_) {
                pattern_graph_->VisitedEdges().Add(*currentEit);
            }

            auto neighbor = currentEit->GetNbr(expand_direction_);

            relp_->path_.Append(currentEit->GetUid());  // add edge's euid to path

            // eit must be valid, set currentNodeId's eiter's needNext to true
            needNext = true;
            stack.emplace_back(ctx, neighbor, currentLevel + 1, relp_, expand_direction_, false,
                               currentLevel + 1 == max_hop_);

            // check predicates here
            for (auto& p: predicates) {
                if (!p->eval(relp_->ItsRef())) {
                    stack.pop_back();
                    relp_->path_.PopBack();
                    relp_->ItsRef()[currentLevel].FreeIter();
                    break;
                }
            }
        } else {
            // check unique
            if (ctx->path_unique_ && relp_->path_.Length() != 0) {
                CYPHER_THROW_ASSERT(pattern_graph_->VisitedEdges().Erase(
                    relp_->path_.GetNthEdgeWithTid(relp_->path_.Length() - 1)));
            }

            stack.pop_back();
            if (currentLevel >= min_hop_) {
                neighbor_->PushVid(currentNodeId);

                // check label
                if (!neighbor_->Label().empty() && neighbor_->IsValidAfterMaterialize(ctx) &&
                    neighbor_->ItRef()->GetLabel() != neighbor_->Label()) {
                    if (relp_->path_.Length() != 0) {
                        relp_->path_.PopBack();
                    }
                    continue;
                }

                if (relp_->path_.Length() != 0) {
                    needPop = true;
                }

                return true;
            }
            if (relp_->path_.Length() != 0) {
                relp_->path_.PopBack();
            }
        }
    }
    return false;
}

VarLenExpand::VarLenExpand(PatternGraph *pattern_graph, Node *start, Node *neighbor,
                           Relationship *relp)
    : OpBase(OpType::VAR_LEN_EXPAND, "Variable Length Expand"),
      pattern_graph_(pattern_graph),
      start_(start),
      neighbor_(neighbor),
      relp_(relp),
      min_hop_(relp->MinHop()),
      max_hop_(relp->MaxHop()) {
    modifies.emplace_back(neighbor_->Alias());
    modifies.emplace_back(relp_->Alias());
    auto &sym_tab = pattern_graph->symbol_table;
    auto sit = sym_tab.symbols.find(start_->Alias());
    auto dit = sym_tab.symbols.find(neighbor_->Alias());
    auto rit = sym_tab.symbols.find(relp_->Alias());
    CYPHER_THROW_ASSERT(sit != sym_tab.symbols.end() && dit != sym_tab.symbols.end() &&
                        rit != sym_tab.symbols.end());
    expand_direction_ = relp_->Undirected()            ? BIDIRECTIONAL
                        : relp_->Src() == start_->ID() ? FORWARD
                                                       : REVERSED;
    start_rec_idx_ = sit->second.id;
    nbr_rec_idx_ = dit->second.id;
    relp_rec_idx_ = rit->second.id;
}

void VarLenExpand::addPredicate(std::unique_ptr<Predicate> p) {
    predicates.push_back(std::move(p));
}

void VarLenExpand::PushFilter(std::shared_ptr<lgraph::Filter> filter) {
    if (filter) {
        if (filter->Type() == lgraph::Filter::RANGE_FILTER) {
            std::shared_ptr<lgraph::RangeFilter> tmp_filter =
                std::static_pointer_cast<lgraph::RangeFilter>(filter);
            if (tmp_filter->GetAeLeft().op.type == cypher::ArithOpNode::AR_OP_FUNC) {
                std::string func_name = tmp_filter->GetAeLeft().op.func_name;
                std::transform(func_name.begin(), func_name.end(), func_name.begin(), ::tolower);
                if (func_name == "isasc") {
                    // auto p = std::make_unique<IsAscPredicate>();
                    // addPredicate(std::move(p));
                } else if (func_name == "isdesc") {
                    // std::unique_ptr<Predicate> p(new IsDescPredicate());
                    // addPredicate(std::move(p));
                } else if (func_name == "head") {
                    lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    FieldData operand = tmp_filter->GetAeRight().operand.constant;
                    auto p = std::make_unique<HeadPredicate>(op, operand);
                    addPredicate(std::move(p));
                } else if (func_name == "last") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                    // std::unique_ptr<Predicate> p(new LastPredicate(op, std::stoi(operand)));
                    // addPredicate(std::move(p));
                } else if (func_name == "maxinlist") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                    // std::unique_ptr<Predicate> p(new MaxInListPredicate(op, std::stoi(operand)));
                    // addPredicate(std::move(p));
                } else if (func_name == "mininlist") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                    // std::unique_ptr<Predicate> p(new MinInListPredicate(op, std::stoi(operand)));
                    // addPredicate(std::move(p));
                }
            }
        }
        PushFilter(filter->Left());
        PushFilter(filter->Right());
    }
    return;
}

void VarLenExpand::PushDownEdgeFilter(std::shared_ptr<lgraph::Filter> edge_filter) {
    edge_filter_ = edge_filter;
    // add filter to local Predicates
    PushFilter(edge_filter);
}

OpBase::OpResult VarLenExpand::Initialize(RTContext *ctx) {
    CYPHER_THROW_ASSERT(!children.empty());
    auto child = children[0];
    auto res = child->Initialize(ctx);
    if (res != OP_OK) return res;
    record = child->record;
    record->values[start_rec_idx_].type = Entry::NODE;
    record->values[start_rec_idx_].node = start_;
    record->values[nbr_rec_idx_].type = Entry::NODE;
    record->values[nbr_rec_idx_].node = neighbor_;
    record->values[relp_rec_idx_].type = Entry::VAR_LEN_RELP;
    record->values[relp_rec_idx_].relationship = relp_;
    relp_->ItsRef().resize(max_hop_);
    return OP_OK;
}

OpBase::OpResult VarLenExpand::RealConsume(RTContext *ctx) {
    CYPHER_THROW_ASSERT(!children.empty());
    auto child = children[0];
    while (!NextWithFilter(ctx)) {
        auto res = child->Consume(ctx);
        relp_->path_.Clear();
        if (res != OP_OK) {
            return res;
        }
        // init the first of stack
        lgraph::VertexId startVid = start_->PullVid();
        if (startVid < 0) {
            continue;
        }
        CYPHER_THROW_ASSERT(stack.empty());
        stack.emplace_back(ctx, startVid, 0, relp_, expand_direction_, false, !max_hop_);
        if (!PerNodeLimit(ctx, stack.front().count)) {
            continue;
        }
        relp_->path_.SetStart(startVid);
    }
    return OP_OK;
}

OpBase::OpResult VarLenExpand::ResetImpl(bool complete) {
    stack.clear();
    relp_->path_.Clear();

    // std::queue<lgraph::VertexId>().swap(frontier_buffer_);
    // std::queue<Path>().swap(path_buffer_);
    // TODO(anyone) reset modifies
    return OP_OK;
}

std::string VarLenExpand::ToString() const {
    auto towards = expand_direction_ == FORWARD    ? "-->"
                   : expand_direction_ == REVERSED ? "<--"
                                                   : "--";
    std::string edgefilter_str = "VarLenEdgeFilter";
    return fma_common::StringFormatter::Format(
        "{}({}) [{} {}*{}..{} {} {}]", name, "All", start_->Alias(), towards,
        std::to_string(min_hop_), std::to_string(max_hop_), neighbor_->Alias(),
        edge_filter_ ? edgefilter_str.append(" (").append(edge_filter_->ToString()).append(")")
                     : "");
}

}  // namespace cypher
