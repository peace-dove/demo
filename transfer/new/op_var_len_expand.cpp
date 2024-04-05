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
// Modified by bxj on 24-3-30.
//

#include "cypher/execution_plan/ops/op_var_len_expand.h"

namespace cypher {

void myPrint(std::string s) { std::cout << s << std::endl; }

// DFS State Class
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
        // if reach max hop, do not init eiter
        (relp->ItsRef()[level]).Initialize(ctx->txn_->GetTxn().get(), iter_type, id, types);
        currentEit = &(relp->ItsRef()[level]);
    }
}

void DfsState::getTime() {
    if (!currentEit->IsValid()) {
        return;
    }
    timestamp = lgraph::FieldData(currentEit->GetField("timestamp"));
    std::cout << "Now edge is: " + currentEit->GetUid().ToString() +
                     ", timestamp is:" + timestamp.ToString()
              << std::endl;
}

// Predicate Class
bool HeadPredicate::eval(std::vector<lgraph::EIter> &eits) {
    auto ret = cypher::FieldData::Array(0);
    // get first edge's timestamp, check whether it fits the condition
    for (auto &eit : eits) {
        if (eit.IsValid()) {
            ret.array->emplace_back(lgraph::FieldData(eit.GetField("timestamp")));
        }
    }
    if (ret.array->empty()) {
        return true;
    }
    FieldData head = FieldData(ret.array->front());
    switch (op) {
    case lgraph::CompareOp::LBR_GT:
        return head > operand;
    case lgraph::CompareOp::LBR_GE:
        return head >= operand;
    case lgraph::CompareOp::LBR_LT:
        return head < operand;
    case lgraph::CompareOp::LBR_LE:
        return head <= operand;
    case lgraph::CompareOp::LBR_EQ:
        return head == operand;
    case lgraph::CompareOp::LBR_NEQ:
        return head != operand;
    default:
        break;
    }
    return false;
}

bool LastPredicate::eval(std::vector<lgraph::EIter> &eits) {
    auto ret = cypher::FieldData::Array(0);
    // get last edge's timestamp, check whether it fits the condition
    for (auto &eit : eits) {
        if (eit.IsValid()) {
            ret.array->emplace_back(lgraph::FieldData(eit.GetField("timestamp")));
        }
    }
    if (ret.array->empty()) {
        return true;
    }
    FieldData last = FieldData(ret.array->back());
    switch (op) {
    case lgraph::CompareOp::LBR_GT:
        return last > operand;
    case lgraph::CompareOp::LBR_GE:
        return last >= operand;
    case lgraph::CompareOp::LBR_LT:
        return last < operand;
    case lgraph::CompareOp::LBR_LE:
        return last <= operand;
    case lgraph::CompareOp::LBR_EQ:
        return last == operand;
    case lgraph::CompareOp::LBR_NEQ:
        return last != operand;
    default:
        break;
    }
    return false;
}

bool IsAscPredicate::eval(std::vector<lgraph::EIter> &eits) {
    auto ret = cypher::FieldData::Array(0);
    for (auto &eit : eits) {
        if (eit.IsValid()) {
            ret.array->emplace_back(lgraph::FieldData(eit.GetField("timestamp")));
        }
    }
    if (ret.array->empty()) {
        return true;
    }
    for (size_t i = 1; i < ret.array->size(); i++) {
        if ((*ret.array)[i - 1] >= (*ret.array)[i]) {
            return false;
        }
    }
    return true;
}

bool IsAscPredicate::eval(std::vector<DfsState> &stack) {
    myPrint("in Asc predicate");

    if (stack.empty()) {
        // length is 0
        return true;
    }
    // length >= 1
    if (!stack.back().currentEit->IsValid()) {
        return false;
    }
    if (stack.size() == 1) {
        return true;
    }
    auto it = stack.end();
    if ((it - 1)->timestamp > (it - 2)->timestamp) {
        // is asc
        myPrint("is asc");
        return true;
    }
    myPrint("not asc");
    return false;
}

bool IsDescPredicate::eval(std::vector<lgraph::EIter> &eits) {
    auto ret = cypher::FieldData::Array(0);
    for (auto &eit : eits) {
        if (eit.IsValid()) {
            ret.array->emplace_back(lgraph::FieldData(eit.GetField("timestamp")));
        }
    }
    if (ret.array->empty()) {
        return true;
    }
    for (size_t i = 1; i < ret.array->size(); i++) {
        if ((*ret.array)[i - 1] <= (*ret.array)[i]) {
            return false;
        }
    }
    return true;
}

bool MaxInListPredicate::eval(std::vector<lgraph::EIter> &eits) {
    auto ret = cypher::FieldData::Array(0);
    for (auto &eit : eits) {
        if (eit.IsValid()) {
            ret.array->emplace_back(lgraph::FieldData(eit.GetField("timestamp")));
        }
    }
    if (ret.array->empty()) {
        return true;
    }
    // find max in path
    size_t pos = 0;
    for (size_t i = 0; i < ret.array->size(); i++) {
        if ((*ret.array)[i] > (*ret.array)[pos]) {
            pos = i;
        }
    }

    FieldData maxInList = cypher::FieldData((*ret.array)[pos]);
    switch (op) {
    case lgraph::CompareOp::LBR_GT:
        return maxInList > operand;
    case lgraph::CompareOp::LBR_GE:
        return maxInList >= operand;
    case lgraph::CompareOp::LBR_LT:
        return maxInList < operand;
    case lgraph::CompareOp::LBR_LE:
        return maxInList <= operand;
    case lgraph::CompareOp::LBR_EQ:
        return maxInList == operand;
    case lgraph::CompareOp::LBR_NEQ:
        return maxInList != operand;
    default:
        break;
    }
    return false;
}

bool MinInListPredicate::eval(std::vector<lgraph::EIter> &eits) {
    auto ret = cypher::FieldData::Array(0);
    for (auto &eit : eits) {
        if (eit.IsValid()) {
            ret.array->emplace_back(lgraph::FieldData(eit.GetField("timestamp")));
        }
    }
    if (ret.array->empty()) {
        return true;
    }
    // find min in path
    size_t pos = 0;
    for (size_t i = 0; i < ret.array->size(); i++) {
        if ((*ret.array)[i] < (*ret.array)[pos]) {
            pos = i;
        }
    }

    FieldData minInList = cypher::FieldData((*ret.array)[pos]);
    switch (op) {
    case lgraph::CompareOp::LBR_GT:
        return minInList > operand;
    case lgraph::CompareOp::LBR_GE:
        return minInList >= operand;
    case lgraph::CompareOp::LBR_LT:
        return minInList < operand;
    case lgraph::CompareOp::LBR_LE:
        return minInList <= operand;
    case lgraph::CompareOp::LBR_EQ:
        return minInList == operand;
    case lgraph::CompareOp::LBR_NEQ:
        return minInList != operand;
    default:
        break;
    }
    return false;
}

// VarLenExpand Class
bool VarLenExpand::PerNodeLimit(RTContext *ctx, size_t count) {
    return !ctx->per_node_limit_.has_value() || count <= ctx->per_node_limit_.value();
}

bool VarLenExpand::NextWithFilter(RTContext *ctx) {
    while (!stack.empty()) {
        auto &currentState = stack.back();
        // auto currentNodeId = currentState.currentNodeId;
        auto &currentEit = currentState.currentEit;
        auto currentLevel = currentState.level;

        // TODO the part of count, needs check
        auto &currentCount = currentState.count;
        // if (!PerNodeLimit(ctx, currentCount)) {
        //     stack.pop_back();
        //     if (relp_->path_.Length() != 0) {
        //         needPop = true;
        //     }
        //     continue;
        // }

        // if currentNodeId's needNext is true, currentEit.next, then set needNext to false
        auto &needNext = currentState.needNext;
        if (needNext) {
            CYPHER_THROW_ASSERT(currentEit->IsValid());
            // check unique, delete previous edge
            if (ctx->path_unique_ && relp_->path_.Length() != 0) {
                CYPHER_THROW_ASSERT(pattern_graph_->VisitedEdges().Erase(
                    relp_->path_.GetNthEdgeWithTid(relp_->path_.Length() - 1)));
            }
            relp_->path_.PopBack();
            
            currentEit->Next();
            currentState.getTime();
            currentCount++;

            bool isFinding = true;
            while (isFinding) {
                std::cout << "before predicate:" << std::endl;
                bool continueFind = false;
                // check predicates here, path derived from eiters in stack
                for (auto &p : predicates) {
                    if (!p->eval(stack)) {
                        // not fit predicate
                        continueFind = true;
                        if (stack.back().currentEit->IsValid()) {
                            // the back eiter is still valid
                            stack.back().currentEit->Next();
                            stack.back().getTime();
                            stack.back().count++;
                        } else {
                            // now the back eiter of stack is invalid
                            isFinding = false;
                        }
                        break;
                    }
                }
                // reach here, the all predicate is ok
                std::cout << "after predicate:" << std::endl;
                if (continueFind) {
                    continueFind = false;
                    continue;
                }
                // when reach here, the eit, path's predicate are ok
                isFinding = false;

                // add edge's euid to path
                myPrint(relp_->path_.ToString());
                myPrint(stack.back().currentEit->GetUid().ToString());
                myPrint("Before append");
                relp_->path_.Append(stack.back().currentEit->GetUid());

                if (ctx->path_unique_ &&
                    pattern_graph_->VisitedEdges().Contains(*stack.back().currentEit)) {
                    // if this edge has been added, find next edge from the same eiter
                    isFinding = true;
                    // set next
                    stack.back().currentEit->Next();
                    stack.back().getTime();
                    stack.back().count++;
                    // pop path
                    relp_->path_.PopBack();
                } else if (ctx->path_unique_) {
                    // this is ok, add edge to path unique
                    pattern_graph_->VisitedEdges().Add(*stack.back().currentEit);
                }
            }

            needNext = false;
        }

        if (relp_->path_.Length() == currentLevel && currentLevel == max_hop_) {
            if (currentEit->IsValid()) {
                // the back eit is valid
                // check label
                if (!neighbor_->Label().empty() && neighbor_->IsValidAfterMaterialize(ctx) &&
                    neighbor_->ItRef()->GetLabel() != neighbor_->Label()) {
                    if (relp_->path_.Length() != 0) {
                        relp_->path_.PopBack();
                    }
                    continue;
                }

                neighbor_->PushVid(currentEit->GetNbr(expand_direction_));

                needNext = true;

                return true;
            } else {
                // the back eit is invalid
                stack.pop_back();
                continue;
            }
        }

        if (currentEit->IsValid()) {
            // eit is valid, set currentNodeId's eiter's needNext to true
            needNext = true;

            auto neighbor = currentEit->GetNbr(expand_direction_);
            stack.emplace_back(ctx, neighbor, currentLevel + 1, relp_, expand_direction_, false,
                               currentLevel + 1 > max_hop_);
            stack.back().getTime();

            bool isFinding = true;
            while (isFinding) {
                std::cout << "before predicate:" << std::endl;
                bool continueFind = false;
                // check predicates here, path derived from eiters in stack
                for (auto &p : predicates) {
                    if (!p->eval(stack)) {
                        // not fit predicate
                        continueFind = true;
                        if (stack.back().currentEit->IsValid()) {
                            // the back eiter is still valid
                            stack.back().currentEit->Next();
                            stack.back().getTime();
                            stack.back().count++;
                        } else {
                            // now the back eiter of stack is invalid
                            isFinding = false;
                        }
                        break;
                    }
                }
                // reach here, the all predicate is ok
                std::cout << "after predicate:" << std::endl;
                if (continueFind) {
                    continueFind = false;
                    continue;
                }
                // when reach here, the eit, path's predicate are ok
                isFinding = false;

                // add edge's euid to path
                myPrint(relp_->path_.ToString());
                myPrint(stack.back().currentEit->GetUid().ToString());
                myPrint("Before append");
                relp_->path_.Append(stack.back().currentEit->GetUid());

                if (ctx->path_unique_ &&
                    pattern_graph_->VisitedEdges().Contains(*stack.back().currentEit)) {
                    // if this edge has been added, find next edge
                    isFinding = true;
                    // set next
                    stack.back().currentEit->Next();
                    stack.back().getTime();
                    stack.back().count++;
                    // pop path
                    relp_->path_.PopBack();
                } else if (ctx->path_unique_) {
                    // this is ok, add edge
                    pattern_graph_->VisitedEdges().Add(*stack.back().currentEit);
                }
            }

        } else {
            // now the eit is vaild
            stack.pop_back();
            if (relp_->path_.Length() == currentLevel && currentLevel >= min_hop_) {
                // check label
                if (!neighbor_->Label().empty() && neighbor_->IsValidAfterMaterialize(ctx) &&
                    neighbor_->ItRef()->GetLabel() != neighbor_->Label()) {
                    if (relp_->path_.Length() != 0) {
                        relp_->path_.PopBack();
                    }
                    continue;
                }

                neighbor_->PushVid(relp_->path_.GetNthEdge(relp_->path_.Length() - 1).dst);

                // if (relp_->path_.Length() != 0) {
                //     needPop = true;
                // }

                return true;
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
                    auto p = std::make_unique<IsAscPredicate>();
                    addPredicate(std::move(p));
                } else if (func_name == "isdesc") {
                    // auto p = std::make_unique<IsDescPredicate>();
                    // addPredicate(std::move(p));
                } else if (func_name == "head") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // FieldData operand = tmp_filter->GetAeRight().operand.constant;
                    // auto p = std::make_unique<HeadPredicate>(op, operand);
                    // addPredicate(std::move(p));
                } else if (func_name == "last") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // FieldData operand = tmp_filter->GetAeRight().operand.constant;
                    // auto p = std::make_unique<LastPredicate>(op, operand);
                    // addPredicate(std::move(p));
                } else if (func_name == "maxinlist") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // FieldData operand = tmp_filter->GetAeRight().operand.constant;
                    // auto p = std::make_unique<MaxInListPredicate>(op, operand);
                    // addPredicate(std::move(p));
                } else if (func_name == "mininlist") {
                    // lgraph::CompareOp op = tmp_filter->GetCompareOp();
                    // FieldData operand = tmp_filter->GetAeRight().operand.constant;
                    // auto p = std::make_unique<MinInListPredicate>(op, operand);
                    // addPredicate(std::move(p));
                } else {
                    throw lgraph::CypherException("Not in 6 predicates.");
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
    // needPop = false;
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
        // push the first node and the related eiter into the stack
        // it means a node and the related edge is chosen, path length is 1
        stack.emplace_back(ctx, startVid, 1, relp_, expand_direction_, false, 1 > max_hop_);
        stack.back().getTime();

        bool nextNode = false;
        bool isFinding = true;
        while (isFinding) {
            // check predicates here, path derived from eiters in stack
            bool continueCheck = false;
            for (auto &p : predicates) {
                if (!p->eval(stack)) {
                    // when the edge in the stack not fit
                    if (stack.back().currentEit->IsValid()) {
                        // still vaild
                        stack.back().currentEit->Next();
                        stack.back().getTime();
                        stack.back().count++;
                        continueCheck = true;
                    } else {
                        // this eiter is not valid, find next node
                        stack.pop_back();
                        nextNode = true;
                    }
                    break;
                }
            }
            if (continueCheck) {
                continue;
            } else {
                // if ok, it means this path is ok, find next hop
                isFinding = false;
            }
        }
        if (nextNode) {
            // find next node
            continue;
        }
        // when reach here, the first node and eiter are ok

        if (!PerNodeLimit(ctx, stack.front().count)) {
            stack.pop_back();
            continue;
        }
        relp_->path_.SetStart(startVid);
        relp_->path_.Append(stack.back().currentEit->GetUid());
        if (ctx->path_unique_) {
            pattern_graph_->VisitedEdges().Add(*stack.back().currentEit);
        }
    }
    return OP_OK;
}

OpBase::OpResult VarLenExpand::ResetImpl(bool complete) {
    std::vector<DfsState>().swap(stack);
    // stack.clear();
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