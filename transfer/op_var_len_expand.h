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

#include <memory>
#include "core/data_type.h"
#include "cypher/execution_plan/ops/op.h"
#include "filter/filter.h"

#ifndef NDEBUG
#define VAR_LEN_EXP_DUMP_FOR_DEBUG()                                                         \
    do {                                                                                     \
        FMA_DBG() << __func__ << __LINE__ << ": hop=" << hop_ << ", edge="                   \
                  << (hop_ == 0 ? "na" : _detail::EdgeUid2String(eits_[hop_ - 1].GetUid())); \
        FMA_DBG() << pattern_graph_->VisitedEdges().Dump();                                  \
    } while (0)
#else
#define VAR_LEN_EXP_DUMP_FOR_DEBUG()
#endif

namespace cypher {

struct DfsState {
    // current node id
    lgraph::VertexId currentNodeId;
    // current index for current node
    lgraph::EIter &currentEit;
    // level, or path length
    int level;

    // int count;
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

// TODO

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
    void _InitializeEdgeIter(RTContext *ctx, int64_t vid, lgraph::EIter &eit, size_t &count) {
        auto &types = relp_->Types();
        auto iter_type = lgraph::EIter::NA;
        switch (expand_direction_) {
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
        eit.Initialize(ctx->txn_->GetTxn().get(), iter_type, vid, types);
        count = 1;
    }

    bool PerNodeLimit(RTContext *ctx, size_t k) {
        return !ctx->per_node_limit_.has_value() ||
               expand_counts_[k] <= ctx->per_node_limit_.value();
    }

    int64_t GetFirstFromKthHop(RTContext *ctx, size_t k) {
        auto start_id = start_->PullVid();
        relp_->path_.Clear();
        relp_->path_.SetStart(start_id);
        if (k == 0) return start_id;
        _InitializeEdgeIter(ctx, start_id, eits_[0], expand_counts_[0]);
        if (!eits_[0].IsValid() || !PerNodeLimit(ctx, 0)) {
            return -1;
        }
        if (k == 1) {
            relp_->path_.Append(eits_[0].GetUid());
            if (ctx->path_unique_) pattern_graph_->VisitedEdges().Add(eits_[0]);
            return eits_[0].GetNbr(expand_direction_);
        }
        // k >= 2
        for (size_t i = 0; i < k; i++) {
            lgraph::EdgeUid dummy(start_id, start_id, -1, 0, -1);
            relp_->path_.Append(dummy);
        }
        return GetNextFromKthHop(ctx, k, true);
    }

    // curr_hop start from 1,2,3..
    int64_t GetNextFromKthHop(RTContext *ctx, size_t k, bool get_first) {
        if (k == 0) return -1;
        if (ctx->path_unique_) pattern_graph_->VisitedEdges().Erase(eits_[k - 1]);
        relp_->path_.PopBack();
        /* If get the first node, the 1st edge(eits[0]) is the only iterator
         * that is initialized and should not go next.
         **/
        if (!get_first || k != 1 ||
            (ctx->path_unique_ && pattern_graph_->VisitedEdges().Contains(eits_[k - 1]))) {
            do {
                expand_counts_[k - 1] += 1;
                eits_[k - 1].Next();
            } while (eits_[k - 1].IsValid() && PerNodeLimit(ctx, k - 1) && ctx->path_unique_ &&
                     pattern_graph_->VisitedEdges().Contains(eits_[k - 1]));
        }
        do {
            if (!eits_[k - 1].IsValid() || !PerNodeLimit(ctx, k - 1)) {
                auto id = GetNextFromKthHop(ctx, k - 1, get_first);
                if (id < 0) return id;
                _InitializeEdgeIter(ctx, id, eits_[k - 1], expand_counts_[k - 1]);
                /* We have called get_next previously, mark get_first as
                 * false. */
                get_first = false;
            }
            while (ctx->path_unique_ && pattern_graph_->VisitedEdges().Contains(eits_[k - 1])) {
                expand_counts_[k - 1] += 1;
                eits_[k - 1].Next();
            }
        } while (!eits_[k - 1].IsValid() || !PerNodeLimit(ctx, k - 1));
        if (!eits_[k - 1].IsValid() || !PerNodeLimit(ctx, k - 1)) {
            return -1;
        }
        relp_->path_.Append(eits_[k - 1].GetUid());
        if (ctx->path_unique_) pattern_graph_->VisitedEdges().Add(eits_[k - 1]);
        return eits_[k - 1].GetNbr(expand_direction_);
    }

    OpResult NextWithoutLabelFilter(RTContext *ctx) {
        if (state_ == Uninitialized) return OP_REFRESH;
        /* Start node iterator may be invalid, such as when the start is an argument
         * produced by OPTIONAL MATCH.  */
        if (start_->PullVid() < 0) return OP_REFRESH;
        if (state_ == Resetted) {
            // go to min_hop
            hop_ = min_hop_;
            int64_t nbr_id = GetFirstFromKthHop(ctx, hop_);
            if (nbr_id < 0) return OP_REFRESH;
            neighbor_->PushVid(nbr_id);
            VAR_LEN_EXP_DUMP_FOR_DEBUG();
            state_ = Consuming;
            return OP_OK;
        }
        auto vid = GetNextFromKthHop(ctx, hop_, false);
        if (vid >= 0) {
            neighbor_->PushVid(vid);
            VAR_LEN_EXP_DUMP_FOR_DEBUG();
            return OP_OK;
        } else {
            // need expand to next hop
            if (hop_ == max_hop_) return OP_REFRESH;
            hop_++;
            auto vid = GetFirstFromKthHop(ctx, hop_ - 1);
            if (vid < 0) return OP_REFRESH;
            if (hop_ > 1 && !eits_[hop_ - 2].IsValid()) CYPHER_INTL_ERR();
            _InitializeEdgeIter(ctx, vid, eits_[hop_ - 1], expand_counts_[hop_ - 1]);
            // TODO(anyone) merge these code similiar to GetNextFromKthHop
            do {
                if (!eits_[hop_ - 1].IsValid() || !PerNodeLimit(ctx, hop_ - 1)) {
                    auto v = GetNextFromKthHop(ctx, hop_ - 1, false);
                    if (v < 0) return OP_REFRESH;
                    _InitializeEdgeIter(ctx, v, eits_[hop_ - 1], expand_counts_[hop_ - 1]);
                }
                while (ctx->path_unique_ &&
                       pattern_graph_->VisitedEdges().Contains(eits_[hop_ - 1])) {
                    expand_counts_[hop_ - 1] += 1;
                    eits_[hop_ - 1].Next();
                }
            } while (!eits_[hop_ - 1].IsValid() || !PerNodeLimit(ctx, hop_ - 1));
            neighbor_->PushVid(eits_[hop_ - 1].GetNbr(expand_direction_));
            relp_->path_.Append(eits_[hop_ - 1].GetUid());
            // TODO(anyone) remove in last hop
            if (ctx->path_unique_) pattern_graph_->VisitedEdges().Add(eits_[hop_ - 1]);
            VAR_LEN_EXP_DUMP_FOR_DEBUG();
            return OP_OK;
        }
    }

    OpResult NextWithFilter(RTContext *ctx) {
        if (start_->PullVid() < 0) return OP_REFRESH;
        while (!stack.empty()) {
            auto &currentState = stack.back();
            auto currentNodeId = currentState.currentNodeId;
            auto &currentEit = currentState.currentEit;
            auto currentLevel = currentState.level;

            if (currentLevel > max_hop_) {
                stack.pop_back();
                currentPath.PopBack();
                continue;
            }

            if (currentEit.IsValid()) {
                auto neighbor = currentEit.GetNbr(expand_direction_);
                lgraph::EIter newEit;
                size_t count;
                _InitializeEdgeIter(ctx, neighbor, newEit, count);
                DfsState nextState = {neighbor, newEit, currentLevel + 1};
                stack.push_back(nextState);
                currentPath.Append(currentEit.GetUid());

                currentEit.Next();
            } else {
                stack.pop_back();
                if (currentLevel >= min_hop_) {
                    neighbor_->PushVid(currentNodeId);
                    relp_->path_ = currentPath;
                    currentPath.PopBack();
                    return OP_OK;
                }
                currentPath.PopBack();
            }
        }
        return OP_DEPLETED;
    }

    OpResult Next(RTContext *ctx) {
        do {
            if (NextWithoutLabelFilter(ctx) != OP_OK) return OP_REFRESH;
        } while (!neighbor_->Label().empty() && neighbor_->IsValidAfterMaterialize(ctx) &&
                 neighbor_->ItRef()->GetLabel() != neighbor_->Label());
        return OP_OK;
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
    int hop_;  // current hop working on
    bool collect_all_;
    ExpandTowards expand_direction_;
    std::vector<lgraph::EIter> &eits_;
    std::vector<size_t> expand_counts_;
    enum State {
        Uninitialized, /* ExpandAll wasn't initialized it. */
        Resetted,      /* ExpandAll was just restarted. */
        Consuming,     /* ExpandAll consuming data. */
    } state_;

    // temp used
    std::shared_ptr<lgraph::Filter> edge_filter_ = nullptr;

    // stack for DFS, current path is currentPath
    std::vector<DfsState> stack;
    Path currentPath;

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
          max_hop_(relp->MaxHop()),
          hop_(0),
          collect_all_(false),
          eits_(relp_->ItsRef()) {
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
        expand_counts_.resize(eits_.size());
        state_ = Uninitialized;
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
        eits_.resize(max_hop_);

        return OP_OK;
    }

    // OpResult RealConsume(RTContext *ctx) override {
    //     CYPHER_THROW_ASSERT(!children.empty());
    //     auto child = children[0];
    //     while (state_ == Uninitialized || Next(ctx) == OP_REFRESH) {
    //         auto res = child->Consume(ctx);
    //         relp_->path_.Clear();
    //         state_ = Resetted;
    //         if (res != OP_OK) {
    //             /* When consume after the stream is DEPLETED, make sure
    //              * the result always be DEPLETED.  */
    //             state_ = Uninitialized;
    //             return res;
    //         }
    //         /* Most of the time, the start_it is definitely valid after child's Consume
    //          * returns OK, except when the child is an OPTIONAL operation.  */
    //     }
    //     return OP_OK;
    // }

    OpResult RealConsume(RTContext *ctx) override {
        CYPHER_THROW_ASSERT(!children.empty());
        auto child = children[0];
        while (NextWithFilter(ctx) != OP_OK) {
            auto res = child->Consume(ctx);
            relp_->path_.Clear();
            if (start_->PullVid() < 0) continue;

            if (res != OP_OK) {
                /* When consume after the stream is DEPLETED, make sure
                 * the result always be DEPLETED.  */
                state_ = Uninitialized;
                return res;
            }
            // init the first of stack
            lgraph::EIter start_eit;
            size_t count;
            lgraph::VertexId start_vid = start_->PullVid();
            _InitializeEdgeIter(ctx, start_vid, start_eit, count);
            stack.push_back({start_vid, start_eit, 0});
        }
        return OP_OK;
    }

    OpResult ResetImpl(bool complete) override {
        state_ = Uninitialized;
        // std::queue<lgraph::VertexId>().swap(frontier_buffer_);
        // std::queue<Path>().swap(path_buffer_);
        hop_ = 0;
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
