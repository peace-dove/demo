// 构造函数
// 压入从给定点出发的所有可能边，和构成的path

// next()
// 循环
// 栈顶取出，目前path保存成一个结果

// 根据取出的对，压所有可能的边，加入需要保证上界，和构成新的path

// 刚才保存的结果范围正确，返回，否则继续循环

// hasnext()
// stack非空

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
// enum class IterState { START, CONSUMING, FINISH };

class Graph {
private:
    int V;
    vector<vector<int>> adjMatrix;

public:
    Graph(int V) : V(V) {
        adjMatrix.resize(V, vector<int>(V, 0));
    }

    // get g[u][v]
    // TODO check in range
    int getW(int u, int v) {
        return adjMatrix[u][v];
    }

    // get number of node
    int getV() {
        return V;
    }

    // add edge
    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
    }

    // print Graph
    void printGraph() {
        cout << "Adjacency Matrix:" << endl;
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
};

class PathIterator {
private:
    pair<int, int> pathLengthRange;

    stack<vector<pair<Edge, Direction>>> stack;
    vector<pair<Edge, Direction>> currentPath;

    int currentNode = -1;

public:
    Graph& graph;
    PathIterator(Graph& g, int start, pair<int, int> pathLengthRange)
        : graph(g), pathLengthRange(pathLengthRange) {
        currentNode = start;
        for (int i = 0; i < g.getV(); i++) {
            if (graph.getW(currentNode, i) != 0) {
                pair<Edge, Direction> newPair;
                newPair.first = {currentNode, i, graph.getW(currentNode, i)};
                newPair.second = Direction::OUT;
                stack.push({newPair});
            }
        }
    }

    void next(vector<pair<Edge, Direction>>& output) {
        while (hasNext()) {
            currentPath = stack.top();
            stack.pop();
            currentNode = currentPath.back().first.t;
            
            int currentPathLen = currentPath.size();

            if (currentPathLen < pathLengthRange.second) {
                for (int i = 0; i < graph.getV(); i++) {
                    if (graph.getW(currentNode, i) != 0) {
                        auto newPath = currentPath;
                        pair<Edge, Direction> newPair;
                        newPair.first = {currentNode, i,
                                         graph.getW(currentNode, i)};
                        newPair.second = Direction::OUT;
                        newPath.push_back(newPair);
                        stack.push(newPath);
                    }
                }
            }
            if (currentPathLen >= pathLengthRange.first &&
                currentPathLen <= pathLengthRange.second) {
                output = currentPath;
                return;
            }
        }
    }

    bool hasNext() {
        return !stack.empty();
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

    int V, E;
    inputFile >> V >> E;
    Graph g(V);
    for (int i = 0; i < E; ++i) {
        int u, v, weight;
        inputFile >> u >> v >> weight;
        g.addEdge(u, v, weight);
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
    while (iter.hasNext()) {
        iter.next(output);
        for (int i = 0; i < output.size(); i++) {
            cout << output[i].first.s;
            if (output[i].second == Direction::OUT) {
                cout << "-[" << output[i].first.w << "]->";

            } else {
                cout << "<-[" << output[i].first.w << "]-";
            }
            cout << output[i].first.t << ' ';
        }
        cout << endl;
        // cout << output.back().first.t << endl;
    }

    return 0;
}
