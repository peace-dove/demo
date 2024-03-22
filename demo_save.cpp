#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

struct Edge {
    int s, t, w;
};

enum class Direction { IN, OUT };
enum class IterState { START, CONSUMING, FINISH };

class Graph {
private:
    int V;                          // 顶点数
    vector<vector<int>> adjMatrix;  // 邻接矩阵表示图
    vector<int> nextNeighbor;  // 存储每个顶点的下一个邻接节点

public:
    Graph(int V) : V(V) {
        adjMatrix.resize(
            V, vector<int>(V, 0));  // 初始化邻接矩阵，所有元素设为0表示无连接
        nextNeighbor.resize(V, 0);  // 初始化 nextNeighbor 向量
    }

    int getW(int u, int v) {
        return adjMatrix[u][v];
    }

    int getV() {
        return V;
    }

    // 添加边
    void addEdge(int u, int v) {
        adjMatrix[u][v] = 1;
    }

    // 打印图的邻接矩阵
    void printGraph() {
        cout << "Adjacency Matrix:" << endl;
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    // 获取下一个邻接节点
    int getNext(int current, const vector<int>& currentPath) {
        for (int i = nextNeighbor[current]; i < V; ++i) {
            if (adjMatrix[current][i] &&
                find(currentPath.begin(), currentPath.end(), i) ==
                    currentPath.end()) {
                nextNeighbor[current] = i + 1;  // 更新下一个邻接节点的位置
                return i;
            }
        }
        return -1;  // 如果没有未访问的邻接节点，返回-1
    }

    // 非递归 深度优先搜索
    void DFS(int start, pair<int, int> pathLengthRange) {
        stack<pair<int, vector<int>>> stack;
        stack.push({start, {start}});

        while (!stack.empty()) {
            int current = stack.top().first;
            vector<int> currentPath = stack.top().second;
            stack.pop();

            int currentPathLength = currentPath.size() - 1;
            if (currentPathLength >= pathLengthRange.first &&
                currentPathLength <= pathLengthRange.second) {
                // 输出路径
                for (int i = 0; i < currentPath.size() - 1; ++i) {
                    cout << currentPath[i] << " -> ";
                }
                cout << currentPath.back() << endl;
            }

            if (currentPathLength >= pathLengthRange.second)
                continue;

            // 获取下一个未访问的邻接节点
            int next = getNext(current, currentPath);
            while (next != -1) {
                vector<int> newPath = currentPath;
                newPath.push_back(next);
                stack.push({next, newPath});
                next = getNext(current, currentPath);
            }
        }
    }
};

class PathIterator {
private:
    int start;
    pair<int, int> pathLengthRange;
    IterState currentstate;
    // stack<pair<int, vector<pair<Edge, Direction>>>> stack;
    // pair<int, vector<pair<Edge, Direction>>> current_stack_unit;
    // vector<pair<Edge, Direction>> currentPath;
    // pair<Edge, Direction> currentPair;

    int currentnode = -1;
    vector<pair<Edge, Direction>> stack;
    vector<pair<Edge, Direction>> currentPath;
    pair<Edge, Direction> currentPair;

    vector<int> nextNeighbor;

    bool has_out = false;

public:
    Graph graph;
    PathIterator(Graph& g, int start, pair<int, int> pathLengthRange)
        : graph(g), start(start), pathLengthRange(pathLengthRange) {
        currentnode = start;
        currentstate = IterState::START;
        nextNeighbor.resize(g.getV(), 0);
    }

    bool next(vector<pair<Edge, Direction>>& output) {
        while (!stack.empty() || currentstate == IterState::START) {
            if (currentstate == IterState::START) {
                for (int i = nextNeighbor[currentnode]; i < graph.getV(); i++) {
                    if (graph.getW(currentnode, i) != 0) {
                        nextNeighbor[currentnode] = i + 1;
                        pair<Edge, Direction> newPair;
                        newPair.first = {currentnode, i,
                                         graph.getW(currentnode, i)};
                        newPair.second = Direction::OUT;
                        stack.push_back(newPair);
                        break;
                    }
                }
                currentstate = IterState::CONSUMING;
                continue;
            }
            
            if (has_out) {
                for (int i = nextNeighbor[currentnode]; i < graph.getV(); i++) {
                    if (graph.getW(currentnode, i) != 0) {
                        nextNeighbor[currentnode] = i + 1;
                        pair<Edge, Direction> newPair;
                        newPair.first = {currentnode, i,
                                         graph.getW(currentnode, i)};
                        newPair.second = Direction::OUT;

                        stack.push_back(newPair);

                        has_out = false;

                        break;
                    }
                }
            }

            currentPair = stack.back();
            currentnode = currentPair.first.t;

            currentPath = stack;
            int currentPathLength = currentPath.size();
            if (has_out == false &&
                currentPathLength >= pathLengthRange.first &&
                currentPathLength <= pathLengthRange.second) {
                // set output and return true
                output = currentPath;
                has_out = true;
                return true;
            }
            if (currentPathLength > pathLengthRange.second) {
                stack.pop_back();
                stack.pop_back();
                has_out = true;
                continue;
            }

            // nextNeighbor[currentnode] = 0;
            // for (int i = nextNeighbor[currentnode]; i < graph.getV(); i++) {
            //     if (graph.getW(currentnode, i) != 0) {
            //         nextNeighbor[currentnode] = i + 1;
            //         pair<Edge, Direction> newPair;
            //         newPair.first = {currentnode, i,
            //                          graph.getW(currentnode, i)};
            //         newPair.second = Direction::OUT;

            //         stack.push_back(newPair);
            //     }
            // }
        }
        return false;
    }
};

PathIterator getIterator(Graph& g, int start, pair<int, int> pathLengthRange) {
    PathIterator pathIter = PathIterator(g, start, pathLengthRange);
    return pathIter;
}

int main() {
    ifstream inputFile("E:\\paper_learning\\24.3.20\\demo\\input.txt");
    if (!inputFile) {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    int V, E;  // 顶点数、边数
    inputFile >> V >> E;

    Graph g(V);

    // 读取边
    for (int i = 0; i < E; ++i) {
        int u, v;
        inputFile >> u >> v;
        g.addEdge(u, v);
    }

    g.printGraph();

    int startVertex;
    pair<int, int> pathLengthRange;
    inputFile >> startVertex >> pathLengthRange.first >> pathLengthRange.second;
    inputFile.close();

    cout << "DFS paths starting from vertex " << startVertex
         << " with path length in range [" << pathLengthRange.first << ", "
         << pathLengthRange.second << "]:" << endl;

    auto iter = getIterator(g, startVertex, pathLengthRange);
    vector<pair<Edge, Direction>> output;
    while (iter.next(output)) {
        for (int i = 0; i < output.size(); ++i) {
            cout << output[i].first.s;
            if (output[i].second == Direction::OUT) {
                cout << " -> ";
            } else {
                cout << " <- ";
            }
        }
        cout << output.back().first.t << endl;
    }

    // g.DFS(startVertex, pathLengthRange);

    return 0;
}
