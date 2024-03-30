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
#pragma once

#include "cypher/execution_plan/ops/op.h"
#include "filter/filter.h"

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
             ExpandTowards expand_direction, bool needNext, bool isMaxHop);
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

    bool NextWithFilter(RTContext *ctx);

    void PushFilter(std::shared_ptr<lgraph::Filter> filter);

    // keep predicates
    std::vector<std::unique_ptr<Predicate>> predicates;
    // add predicate to the vector
    void addPredicate(std::unique_ptr<Predicate> p);

    // stack for DFS
    std::vector<DfsState> stack;

    // this flag decides whether need to pop relp_->Path
    bool needPop = false;

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

    // edge_filter_ is temp used
    std::shared_ptr<lgraph::Filter> edge_filter_ = nullptr;

    VarLenExpand(PatternGraph *pattern_graph, Node *start, Node *neighbor, Relationship *relp);

    void PushDownEdgeFilter(std::shared_ptr<lgraph::Filter> edge_filter);

    OpResult Initialize(RTContext *ctx) override;

    OpResult RealConsume(RTContext *ctx) override;

    OpResult ResetImpl(bool complete) override;

    std::string ToString() const override;

    Node *GetStartNode() const { return start_; }
    Node *GetNeighborNode() const { return neighbor_; }
    Relationship *GetRelationship() const { return relp_; }

    CYPHER_DEFINE_VISITABLE()

    CYPHER_DEFINE_CONST_VISITABLE()
};
}  // namespace cypher
