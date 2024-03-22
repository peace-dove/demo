#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

class Graph {
private:
    int V;                          // 顶点数
    vector<vector<int>> adjMatrix;  // 邻接矩阵表示图
    vector<int> nextNeighbor;  // 记录每个顶点的下一个要访问的邻接节点
    vector<int> visited;

public:
    Graph(int V) : V(V) {
        adjMatrix.resize(
            V, vector<int>(V, 0));  // 初始化邻接矩阵，所有元素设为0表示无连接
        nextNeighbor.resize(V, 0);  // 初始化 nextNeighbor 向量
    }

    // 添加带权重的边
    void addEdge(int u, int v, int weight) {
        adjMatrix[u][v] = weight;
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

    void reset(int current) {
        nextNeighbor[current] = 0;
    }

    // 获取下一个未访问的邻接节点
    int getNext(int current, const vector<pair<int, int>>& currentPath) {
        for (int i = nextNeighbor[current]; i < V; ++i) {
            if (adjMatrix[current][i] != 0 &&
                find_if(currentPath.begin(), currentPath.end(),
                        [i](const pair<int, int>& p) {
                            return p.first == i;
                        }) == currentPath.end()) {
                nextNeighbor[current] = i + 1;  // 更新下一个邻接节点的位置
                return i;
            }
        }
        return -1;  // 如果没有未访问的邻接节点，返回-1
    }

    // 非递归深度优先搜索
    void DFS(int start, pair<int, int> pathLengthRange) {
        stack<pair<int, vector<pair<int, int>>>>
            stack;  // 每个元素表示当前顶点和已访问路径的集合
        stack.push({start, {{start, 0}}});  // 初始路径包含起始顶点和权重为0

        while (!stack.empty()) {
            int current = stack.top().first;
            vector<pair<int, int>> currentPath = stack.top().second;
            stack.pop();

            int currentPathLength = currentPath.size() - 1;
            if (currentPathLength >= pathLengthRange.first &&
                currentPathLength <= pathLengthRange.second) {
                // 输出路径
                cout << "Path: ";
                for (int i = 0; i < currentPath.size(); ++i) {
                    cout << currentPath[i].first;
                    if (i < currentPath.size() - 1) {
                        cout << "-[" << currentPath[i + 1].second << "]->";
                    }
                }
                cout << endl;
            }

            if (currentPathLength >= pathLengthRange.second)
                continue;

            // 获取下一个未访问的邻接节点
            reset(current);
            int next = getNext(current, currentPath);
            while (next != -1) {
                vector<pair<int, int>> newPath = currentPath;
                newPath.emplace_back(next, adjMatrix[current][next]);
                stack.push({next, newPath});
                next = getNext(current, currentPath);
            }
        }
    }

    // 深度优先搜索DFS遍历连通图
    void DFS(int v, int level)  // 以位置v为始点遍历图G
    {
        cout << v << " " << level << endl;
        visited[v] = 1;  // 访问过第v个结点标记为1
        for (int w = 0; w < V; w++) {
            if (!visited[w])  // 对未访问的第w个结点进行访问
                DFS(w, level + 1);
        }
        cout << v << " " << level << endl;
    }
    // 深度优先搜索DFS遍历非连通图
    void DFSTraverse() {
        visited.resize(V, 0);
        for (int i = 0; i < V; i++)
            if (!visited[i])
                DFS(i, 0);  // 对未访问的结点调用DFS
    }
};

int main() {
    ifstream inputFile("a.txt");
    if (!inputFile) {
        cerr << "Failed to open the input file." << endl;
        return 1;
    }

    int V, E;  // 顶点数、边数
    inputFile >> V >> E;

    Graph g(V);

    // 读取带权重的边
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
    // g.DFS(startVertex, pathLengthRange);
    g.DFSTraverse();
    return 0;
}
