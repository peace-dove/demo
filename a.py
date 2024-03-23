import networkx as nx
import matplotlib.pyplot as plt

# 从文件中读取图的描述
def read_graph_from_file(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        num_vertices, num_edges = map(int, lines[0].split())
        edges = [tuple(map(int, line.split())) for line in lines[1:]]
    return num_vertices, num_edges, edges

# 读取图数据
num_vertices, num_edges, edges = read_graph_from_file("graph_data.txt")

# 创建有向图
G = nx.DiGraph()

# 添加边和权重
for edge in edges:
    G.add_edge(edge[0], edge[1], weight=edge[2])

# 绘制图形
pos = nx.spring_layout(G)  # 使用spring_layout
pos[3] = (pos[3][0]+0.1, pos[3][1]+0.1)  # 调整节点3的位置
nx.draw(G, pos, with_labels=True, node_size=700, node_color="skyblue", font_size=20, font_weight="bold")  # 绘制节点
labels = nx.get_edge_attributes(G, "weight")
nx.draw_networkx_edge_labels(G, pos, edge_labels=labels)  # 绘制带权重的边

# 保存图像
plt.savefig("directed_graph.png", format="PNG")

