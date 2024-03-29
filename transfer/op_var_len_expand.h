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
//
#pragma once

#include "core/data_type.h"
#include "cypher/execution_plan/ops/op.h"
#include "cypher_exception.h"
#include "cypher_types.h"
#include "filter/filter.h"
#include "filter/iterator.h"

namespace cypher {

struct DfsState {
    // current node id
    lgraph::VertexId currentNodeId;
    // current index for current node
    lgraph::EIter *currentEit;
    // level, or path length
    int level;
    // number of neighbors of this node
    int count;
    // whether the eiter need Next()
    bool needNext;

    DfsState(RTContext *ctx, lgraph::VertexId id, int level, cypher::Relationship *relp,
             ExpandTowards expand_direction, bool needNext, bool isMaxHop)
        : currentNodeId(id), level(level), count(0), needNext(needNext) {
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
            relp->ItsRef()[level].Initialize(ctx->txn_->GetTxn().get(), iter_type, id, types);
            currentEit = &relp->ItsRef()[level];
        }
    }
};

class Predicate {
 public:
    virtual bool eval(const Path &) = 0;
};

class HeadPredicate : public Predicate {
 private:
    lgraph::CompareOp op;
    int64_t operand;

 public:
    HeadPredicate(lgraph::CompareOp op, int64_t operand) : op(op), operand(operand) {}
    bool eval(const Path &path) {
        if (path.Empty()) return true;
        // get first edge's timestamp, check whether it fits the condition
        int64_t head = path.GetNthEdge(0).tid;
        switch (op) {
        case lgraph::CompareOp::LBR_GT:
            return head > operand;
            break;
        case lgraph::CompareOp::LBR_GE:
            return head >= operand;
            break;
        case lgraph::CompareOp::LBR_LT:
            return head < operand;
            break;
        case lgraph::CompareOp::LBR_LE:
            return head <= operand;
            break;
        case lgraph::CompareOp::LBR_EQ:
            return head == operand;
            break;
        case lgraph::CompareOp::LBR_NEQ:
            return head != operand;
            break;
        default:
            break;
        }
        return false;
    }
};

class LastPredicate : public Predicate {
 private:
    lgraph::CompareOp op;
    int64_t operand;

 public:
    LastPredicate(lgraph::CompareOp op, int64_t operand) : op(op), operand(operand) {}
    bool eval(const Path &path) {
        if (path.Empty()) return true;
        // get first edge's timestamp, check whether it fits the condition
        int64_t last = path.GetNthEdge(path.Length() - 1).tid;
        switch (op) {
        case lgraph::CompareOp::LBR_GT:
            return last > operand;
            break;
        case lgraph::CompareOp::LBR_GE:
            return last >= operand;
            break;
        case lgraph::CompareOp::LBR_LT:
            return last < operand;
            break;
        case lgraph::CompareOp::LBR_LE:
            return last <= operand;
            break;
        case lgraph::CompareOp::LBR_EQ:
            return last == operand;
            break;
        case lgraph::CompareOp::LBR_NEQ:
            return last != operand;
            break;
        default:
            break;
        }
        return false;
    }
};

class IsAscPredicate : public Predicate {
 public:
    IsAscPredicate() {}
    bool eval(const Path &path) {
        if (path.Empty()) return true;
        for (size_t i = 0; i < path.Length() - 1; i++) {
            int64_t a = path.GetNthEdge(i).tid;
            int64_t b = path.GetNthEdge(i + 1).tid;
            if (a >= b) return false;
        }
        return true;
    }
};

class IsDescPredicate : public Predicate {
 public:
    IsDescPredicate() {}
    bool eval(const Path &path) {
        if (path.Empty()) return true;
        for (size_t i = 0; i < path.Length() - 1; i++) {
            int64_t a = path.GetNthEdge(i).tid;
            int64_t b = path.GetNthEdge(i + 1).tid;
            if (a <= b) return false;
        }
        return true;
    }
};

class MaxInListPredicate : public Predicate {
 private:
    lgraph::CompareOp op;
    int64_t operand;

 public:
    MaxInListPredicate(lgraph::CompareOp op, int64_t operand) : op(op), operand(operand) {}
    bool eval(const Path &path) {
        if (path.Empty()) return true;
        int64_t maxinlist = path.GetNthEdge(0).tid;
        for (size_t i = 1; i < path.Length(); i++) {
            maxinlist = std::max(maxinlist, path.GetNthEdge(i).tid);
        }
        switch (op) {
        case lgraph::CompareOp::LBR_GT:
            return maxinlist > operand;
            break;
        case lgraph::CompareOp::LBR_GE:
            return maxinlist >= operand;
            break;
        case lgraph::CompareOp::LBR_LT:
            return maxinlist < operand;
            break;
        case lgraph::CompareOp::LBR_LE:
            return maxinlist <= operand;
            break;
        case lgraph::CompareOp::LBR_EQ:
            return maxinlist == operand;
            break;
        case lgraph::CompareOp::LBR_NEQ:
            return maxinlist != operand;
            break;
        default:
            break;
        }
        return false;
    }
};

class MinInListPredicate : public Predicate {
 private:
    lgraph::CompareOp op;
    int64_t operand;

 public:
    MinInListPredicate(lgraph::CompareOp op, int64_t operand) : op(op), operand(operand) {}
    bool eval(const Path &path) {
        if (path.Empty()) return true;
        int64_t mininlist = path.GetNthEdge(0).tid;
        for (size_t i = 1; i < path.Length(); i++) {
            mininlist = std::min(mininlist, path.GetNthEdge(i).tid);
        }
        switch (op) {
        case lgraph::CompareOp::LBR_GT:
            return mininlist > operand;
            break;
        case lgraph::CompareOp::LBR_GE:
            return mininlist >= operand;
            break;
        case lgraph::CompareOp::LBR_LT:
            return mininlist < operand;
            break;
        case lgraph::CompareOp::LBR_LE:
            return mininlist <= operand;
            break;
        case lgraph::CompareOp::LBR_EQ:
            return mininlist == operand;
            break;
        case lgraph::CompareOp::LBR_NEQ:
            return mininlist != operand;
            break;
        default:
            break;
        }
        return false;
    }
};

/* Variable Length Expand */
class VarLenExpand : public OpBase {
    // bool PerNodeLimit(RTContext *ctx, size_t k) {
    //     return !ctx->per_node_limit_.has_value() ||
    //            expand_counts_[k] <= ctx->per_node_limit_.value();
    // }

    bool NextWithFilter(RTContext *ctx) {
        while (!stack.empty()) {
            if (needPop) {
                relp_->path_.PopBack();
                // relp_->ItsRef().pop_back();
                needPop = false;
            }
            auto &currentState = stack.back();
            auto currentNodeId = currentState.currentNodeId;
            auto &currentEit = currentState.currentEit;
            auto currentLevel = currentState.level;

            auto &needNext = currentState.needNext;

            // if currentNodeId's needNext = true, currentEit.next()
            // then set needNext = false
            if (needNext) {
                currentEit->Next();
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
                        // relp_->ItsRef().pop_back();
                    }
                    continue;
                }

                if (relp_->path_.Length() != 0) {
                    needPop = true;
                }
                std::cout << relp_->path_.ToString() << std::endl;

                return true;
            }

            if (currentEit->IsValid()) {
                // check path unique
                if (ctx->path_unique_ &&
                    pattern_graph_->VisitedEdges().Contains(currentEit->GetUid())) {
                    currentEit->Next();
                    continue;
                } else if (ctx->path_unique_) {
                    pattern_graph_->VisitedEdges().Add(*currentEit);
                }

                auto neighbor = currentEit->GetNbr(expand_direction_);

                relp_->path_.Append(currentEit->GetUid());  // add edge's euid to path

                // relp_->ItsRef().push_back(std::move(currentEit));  // add currentNodeId's Eit
                // CYPHER_THROW_ASSERT(currentEit->IsValid());

                // currentNodeId's iter, needNext = true
                needNext = true;
                stack.emplace_back(ctx, neighbor, currentLevel + 1, relp_, expand_direction_, false,
                                   currentLevel + 1 == max_hop_);
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
                            // relp_->ItsRef().pop_back();
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
                    // relp_->ItsRef().pop_back();
                }
            }
        }
        return false;
    }

 public:
    cypher::PatternGraph *pattern_graph_ = nullptr;
    cypher::Node *start_ = nullptr;         // start node to expand
    cypher::Node *neighbor_ = nullptr;      // neighbor of start node
    cypher::Relationship *relp_ = nullptr;  // relationship to expand
    int start_rec_idx_;
    int nbr_rec_idx_;
    int relp_rec_idx_;
    int min_hop_;
    int max_hop_;
    ExpandTowards expand_direction_;

    // temp used
    std::shared_ptr<lgraph::Filter> edge_filter_ = nullptr;

    // stack for DFS
    std::vector<DfsState> stack;
    
    // this flag decides whether need to pop relp_->Path
    bool needPop = false;

    // keep predicates
    std::vector<std::unique_ptr<Predicate>> predicates;
    // add predicate to the vector
    void addPredicate(std::unique_ptr<Predicate> p) { predicates.push_back(std::move(p)); }

    VarLenExpand(PatternGraph *pattern_graph, Node *start, Node *neighbor, Relationship *relp)
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

    void PushFilter(std::shared_ptr<lgraph::Filter> filter) {
        if (filter) {
            if (filter->Type() == lgraph::Filter::RANGE_FILTER) {
                std::shared_ptr<lgraph::RangeFilter> tmp_filter =
                    std::static_pointer_cast<lgraph::RangeFilter>(filter);
                if (tmp_filter->GetAeLeft().op.type == cypher::ArithOpNode::AR_OP_FUNC) {
                    std::string func_name = tmp_filter->GetAeLeft().op.func_name;
                    std::transform(func_name.begin(), func_name.end(), func_name.begin(),
                                   ::tolower);
                    if (func_name == "isasc") {
                        auto p = std::make_unique<IsAscPredicate>();
                        // std::unique_ptr<Predicate> p(new IsAscPredicate());
                        addPredicate(std::move(p));
                    } else if (func_name == "isdesc") {
                        std::unique_ptr<Predicate> p(new IsDescPredicate());
                        addPredicate(std::move(p));
                    } else if (func_name == "head") {
                        lgraph::CompareOp op = tmp_filter->GetCompareOp();
                        auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                        std::unique_ptr<Predicate> p(new HeadPredicate(op, std::stoi(operand)));
                        addPredicate(std::move(p));
                    } else if (func_name == "last") {
                        lgraph::CompareOp op = tmp_filter->GetCompareOp();
                        auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                        std::unique_ptr<Predicate> p(new LastPredicate(op, std::stoi(operand)));
                        addPredicate(std::move(p));
                    } else if (func_name == "maxinlist") {
                        lgraph::CompareOp op = tmp_filter->GetCompareOp();
                        auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                        std::unique_ptr<Predicate> p(
                            new MaxInListPredicate(op, std::stoi(operand)));
                        addPredicate(std::move(p));
                    } else if (func_name == "mininlist") {
                        lgraph::CompareOp op = tmp_filter->GetCompareOp();
                        auto operand = tmp_filter->GetAeRight().operand.constant.ToString();
                        std::unique_ptr<Predicate> p(
                            new MinInListPredicate(op, std::stoi(operand)));
                        addPredicate(std::move(p));
                    }
                }
            }
            PushFilter(filter->Left());
            PushFilter(filter->Right());
        }
        return;
    }

    void PushDownEdgeFilter(std::shared_ptr<lgraph::Filter> edge_filter) {
        edge_filter_ = edge_filter;
        // add filter to local Predicates
        PushFilter(edge_filter);
    }

    OpResult Initialize(RTContext *ctx) override {
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

    OpResult RealConsume(RTContext *ctx) override {
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
            relp_->path_.SetStart(startVid);
        }
        return OP_OK;
    }

    OpResult ResetImpl(bool complete) override {
        stack.clear();
        relp_->path_.Clear();
        relp_->ItsRef().clear();
        // std::queue<lgraph::VertexId>().swap(frontier_buffer_);
        // std::queue<Path>().swap(path_buffer_);
        // TODO(anyone) reset modifies
        return OP_OK;
    }

    std::string ToString() const override {
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

    Node *GetStartNode() const { return start_; }
    Node *GetNeighborNode() const { return neighbor_; }
    Relationship *GetRelationship() const { return relp_; }

    CYPHER_DEFINE_VISITABLE()

    CYPHER_DEFINE_CONST_VISITABLE()
};
}  // namespace cypher
