#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

struct Edge {
    int s, t, w;
};

enum class Direction { IN, OUT };
enum class CompareOp { GT, GE, LT, LE, EQ, NE };
// enum class IterState { START, CONSUMING, FINISH };

class Graph {
private:
    vector<vector<pair<int, int>>> outgoingAdjList;
    vector<vector<pair<int, int>>> incomingAdjList;

public:
    Graph(int V) : outgoingAdjList(V), incomingAdjList(V) {
    }

    int getV() {
        return outgoingAdjList.size();
    }

    int getOutgoingNeighborCount(int i) {
        return outgoingAdjList[i].size();
    }

    int getIncomingNeighborCount(int i) {
        return incomingAdjList[i].size();
    }

    int getOutgoingNeighbor(int node, int index) {
        return outgoingAdjList[node][index].first;
    }

    int getIncomingNeighbor(int node, int index) {
        return incomingAdjList[node][index].first;
    }

    int getOutgoingWeight(int node, int index) {
        return outgoingAdjList[node][index].second;
    }

    int getIncomingWeight(int node, int index) {
        return incomingAdjList[node][index].second;
    }

    void addEdge(int u, int v, int weight) {
        outgoingAdjList[u].push_back(make_pair(v, weight));
        incomingAdjList[v].push_back(make_pair(u, weight));
    }

    void printGraph() {
        for (int i = 0; i < getV(); i++) {
            cout << i << ": out ";
            for (auto v : outgoingAdjList[i]) {
                cout << v.first << ' ';
            }
            cout << endl;
            cout << i << ": in ";
            for (auto v : incomingAdjList[i]) {
                cout << v.first << ' ';
            }
            cout << endl;
        }
    }
};

struct DfsState {
    int currentNode;
    int neighborIndex;
    int level;
};

class Predicate {
public:
    virtual bool eval(const vector<pair<Edge, Direction>>&) = 0;
};

class HeadPredicate : public Predicate {
private:
    CompareOp op;
    int operand;

public:
    HeadPredicate(CompareOp op, int operand) : op(op), operand(operand) {
    }
    bool eval(const vector<pair<Edge, Direction>>& path) override {
        if (path.empty())
            return true;
        int head = path.front().first.w;
        switch (op) {
            case CompareOp::GT:
                return head > operand;
                break;
            case CompareOp::GE:
                return head >= operand;
                break;
            case CompareOp::LT:
                return head < operand;
                break;
            case CompareOp::LE:
                return head <= operand;
                break;
            case CompareOp::EQ:
                return head == operand;
                break;
            case CompareOp::NE:
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
    CompareOp op;
    int operand;

public:
    LastPredicate(CompareOp op, int operand) : op(op), operand(operand) {
    }
    bool eval(const vector<pair<Edge, Direction>>& path) override {
        if (path.empty())
            return true;
        int last = path.back().first.w;
        switch (op) {
            case CompareOp::GT:
                return last > operand;
                break;
            case CompareOp::GE:
                return last >= operand;
                break;
            case CompareOp::LT:
                return last < operand;
                break;
            case CompareOp::LE:
                return last <= operand;
                break;
            case CompareOp::EQ:
                return last == operand;
                break;
            case CompareOp::NE:
                return last != operand;
                break;

            default:
                break;
        }
        return false;
    }
};

class AscPredicate : public Predicate {
public:
    bool eval(const vector<pair<Edge, Direction>>& path) override {
        if (path.empty())
            return true;
        for (int i = 0; i < path.size() - 1; i++) {
            if (path[i].first.w >= path[i + 1].first.w) {
                return false;
            }
        }
        return true;
    }
};

class DescPredicate : public Predicate {
public:
    bool eval(const vector<pair<Edge, Direction>>& path) override {
        if (path.empty())
            return true;
        for (int i = 0; i < path.size() - 1; i++) {
            if (path[i].first.w <= path[i + 1].first.w) {
                return false;
            }
        }
        return true;
    }
};

class MinPredicate : public Predicate {
private:
    CompareOp op;
    int operand;

public:
    MinPredicate(CompareOp op, int operand) : op(op), operand(operand) {
    }
    bool eval(const vector<pair<Edge, Direction>>& path) override {
        if (path.empty())
            return true;
        int minWeight = path.front().first.w;
        for (int i = 1; i < path.size(); i++) {
            minWeight =
                minWeight < path[i].first.w ? minWeight : path[i].first.w;
        }
        switch (op) {
            case CompareOp::GT:
                return minWeight > operand;
                break;
            case CompareOp::GE:
                return minWeight >= operand;
                break;
            case CompareOp::LT:
                return minWeight < operand;
                break;
            case CompareOp::LE:
                return minWeight <= operand;
                break;
            case CompareOp::EQ:
                return minWeight == operand;
                break;
            case CompareOp::NE:
                return minWeight != operand;
                break;

            default:
                break;
        }
        return false;
    }
};

class MaxPredicate : public Predicate {
private:
    CompareOp op;
    int operand;

public:
    MaxPredicate(CompareOp op, int operand) : op(op), operand(operand) {
    }
    bool eval(const vector<pair<Edge, Direction>>& path) override {
        if (path.empty())
            return true;
        int maxWeight = path.front().first.w;
        for (int i = 1; i < path.size(); i++) {
            maxWeight =
                maxWeight > path[i].first.w ? maxWeight : path[i].first.w;
        }
        switch (op) {
            case CompareOp::GT:
                return maxWeight > operand;
                break;
            case CompareOp::GE:
                return maxWeight >= operand;
                break;
            case CompareOp::LT:
                return maxWeight < operand;
                break;
            case CompareOp::LE:
                return maxWeight <= operand;
                break;
            case CompareOp::EQ:
                return maxWeight == operand;
                break;
            case CompareOp::NE:
                return maxWeight != operand;
                break;

            default:
                break;
        }
        return false;
    }
};

class PathIterator {
private:
    pair<int, int> pathLengthRange;

    vector<DfsState> stack;

    vector<pair<Edge, Direction>> currentPath;

    vector<unique_ptr<Predicate>> predicates;

public:
    void addPredicate(unique_ptr<Predicate> p) {
        predicates.push_back(move(p));
    }
    Graph& graph;
    PathIterator(Graph& g, int start, pair<int, int> pathLengthRange)
        : graph(g), pathLengthRange(pathLengthRange) {
        stack.push_back({start, 0, 0});
    }

    bool next(vector<pair<Edge, Direction>>& output) {
        while (!stack.empty()) {
            auto& currentState = stack.back();
            auto currentNode = currentState.currentNode;
            auto& neighborIndex = currentState.neighborIndex;
            auto currentLevel = currentState.level;

            if (currentLevel > pathLengthRange.second) {
                stack.pop_back();
                currentPath.pop_back();
                continue;
            }

            auto outgoingNeighborCount =
                graph.getOutgoingNeighborCount(currentNode);
            auto incomingNeighborCount =
                graph.getIncomingNeighborCount(currentNode);

            if (neighborIndex < outgoingNeighborCount) {
                auto neighbor =
                    graph.getOutgoingNeighbor(currentNode, neighborIndex);
                auto weight =
                    graph.getOutgoingWeight(currentNode, neighborIndex);

                DfsState nextState = {neighbor, 0, currentLevel + 1};
                neighborIndex++;
                stack.push_back(nextState);
                pair<Edge, Direction> newPair;
                currentPath.push_back(make_pair<Edge, Direction>(
                    {currentNode, neighbor, weight}, Direction::OUT));

                for (auto& p : predicates) {
                    if (!p->eval(currentPath)) {
                        stack.pop_back();
                        currentPath.pop_back();
                    }
                }

            } else if (neighborIndex <
                       outgoingNeighborCount + incomingNeighborCount) {
                auto neighbor = graph.getIncomingNeighbor(
                    currentNode, neighborIndex - outgoingNeighborCount);
                auto weight = graph.getIncomingWeight(
                    currentNode, neighborIndex - outgoingNeighborCount);

                DfsState nextState = {neighbor, 0, currentLevel + 1};
                neighborIndex++;
                stack.push_back(nextState);
                currentPath.push_back(make_pair<Edge, Direction>(
                    {currentNode, neighbor, weight}, Direction::IN));

                for (auto& p : predicates) {
                    if (!p->eval(currentPath)) {
                        stack.pop_back();
                        currentPath.pop_back();
                    }
                }

            } else {
                stack.pop_back();

                if (currentLevel >= pathLengthRange.first) {
                    output = currentPath;
                    currentPath.pop_back();
                    return true;
                }

                currentPath.pop_back();
            }
        }
        return false;
    }
};

PathIterator getIterator(Graph& g, int start, pair<int, int> pathLengthRange) {
    PathIterator pathIter = PathIterator(g, start, pathLengthRange);
    return pathIter;
}

int main() {
    ifstream inputFile("graph_data.txt");
    if (!inputFile) {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    int V, E;
    inputFile >> V >> E;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v, weight;
        inputFile >> u >> v >> weight;
        g.addEdge(u, v, weight);
    }
    g.printGraph();

    inputFile.close();

    ifstream inputFile2("input.txt");
    if (!inputFile) {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    // start left_range right_range
    int startVertex;
    pair<int, int> pathLengthRange;
    inputFile2 >> startVertex >> pathLengthRange.first >> pathLengthRange.second;

    auto iter = getIterator(g, startVertex, pathLengthRange);

    // number of predicates
    // 0-5(head, last, Asc, Desc, min, max)
    // 0-5(GT, GE, LT, LE, EQ, NE)
    // operand
    int numPredicates;
    vector<CompareOp> opList = {CompareOp::GT, CompareOp::GE, CompareOp::LT,
                                CompareOp::LE, CompareOp::EQ, CompareOp::NE};
    inputFile2 >> numPredicates;
    for (int i = 0; i < numPredicates; i++) {
        int funNum, op, operand;
        inputFile2 >> funNum >> op >> operand;
        if (funNum == 0) {
            // HeadPredicate
            unique_ptr<Predicate> p(new HeadPredicate(opList[op], operand));
            iter.addPredicate(move(p));
        } else if (funNum == 1) {
            // LastPredicate
            unique_ptr<Predicate> p(new LastPredicate(opList[op], operand));
            iter.addPredicate(move(p));
        } else if (funNum == 2) {
            // AscPredicate
            unique_ptr<Predicate> p(new AscPredicate());
            iter.addPredicate(move(p));
        } else if (funNum == 3) {
            // DescPredicate
            unique_ptr<Predicate> p(new DescPredicate());
            iter.addPredicate(move(p));
        } else if (funNum == 4) {
            // MinPredicate
            unique_ptr<Predicate> p(new MinPredicate(opList[op], operand));
            iter.addPredicate(move(p));
        } else if (funNum == 5) {
            // MaxPredicate
            unique_ptr<Predicate> p(new MaxPredicate(opList[op], operand));
            iter.addPredicate(move(p));
        } else {
            cout << "ERROR Input for funNum." << endl;
            inputFile2.close();
            return -1;
        }
    }

    inputFile2.close();

    cout << "DFS paths starting from vertex " << startVertex
         << " with path length in range [" << pathLengthRange.first << ", "
         << pathLengthRange.second << "]:" << endl;

    vector<pair<Edge, Direction>> output;
    while (iter.next(output)) {
        for (int i = 0; i < output.size(); i++) {
            if (i == 0) {
                cout << output[i].first.s;
            }
            if (output[i].second == Direction::OUT) {
                cout << "-[" << output[i].first.w << "]->";

            } else {
                cout << "<-[" << output[i].first.w << "]-";
            }
            cout << output[i].first.t;
        }
        cout << endl;
        // cout << output.back().first.t << endl;
    }

    return 0;
}
