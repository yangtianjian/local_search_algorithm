//
// Created by apple on 2020/6/20.
//

#include "Graph.h"


#include <algorithm>

void Node::add_edge(Node* to) {
    adj.push_back(to);
}

const std::vector<Node*>& Node::get_adjacent() const {
    return adj;
}

bool Graph::edge_exists(int u, int v) {
    Node* node_u = &nodes[u];
    const std::vector<Node*>& adj = node_u -> get_adjacent();
    for(Node* node_v: adj) {
        if(v == node_v -> id) return true;
    }
    return false;
}

void Graph::add_edge(int u, int v) {
    Node* node_v = &nodes[v];
    Node* node_u = &nodes[u];

    if(edge_exists(u, v)) throw edge_exists_exception(u, v);

    if(!directed) {
        nodes[u].add_edge(node_v);
        nodes[v].add_edge(node_u);
    } else {
        nodes[u].add_edge(node_v);
    }
}

std::vector<int> Graph::get_adjacent(int u) const {
    const std::vector<Node*> adj = nodes[u].get_adjacent();
    std::vector<int> ret; ret.reserve(adj.size());
    for(Node* node: adj) {ret.push_back(node -> id);}
    return ret;
}

std::ostream& operator<< (std::ostream& ofs, const Graph& g) {
    int total_n = g.n, m = 0;
    for(const Node& node: g.nodes) m += node.get_adjacent().size();

    if(g.directed) {  // for directed graph

        ofs << total_n << " " << m << std::endl;
        for(const Node& node: g.nodes) {
            int id = node.get_id();
            const std::vector<Node*>& adj = node.get_adjacent();
            for(const Node* adj0: adj) {
                ofs << id << " " << (adj0 -> get_id()) << std::endl;
            }
        }
    } else {
        ofs << total_n << " " << (m >> 1) << std::endl;
        for(const Node& node: g.nodes) {
            int id = node.get_id();
            const std::vector<Node*>& adj = node.get_adjacent();
            for(const Node* adj0: adj) {
                int adj_id = adj0 -> get_id();
                if(id <= adj_id) {
                    ofs << id << " " << (adj0 -> get_id()) << std::endl;
                }
            }
        }
    }

    return ofs;
}

std::istream& operator>> (std::istream& ifs, Graph& g) {
    int m;
    ifs >> g.n >> m;
    g.init_nodes(g.n);
    for(int i = 0; i < m; ++i) {
        int u, v;
        ifs >> u >> v;
        g.add_edge(u, v);
        if(g.directed) g.add_edge(v, u);
    }
    return ifs;
}