//
// Created by apple on 2020/6/20.
//

#ifndef CODE_GRAPH_H
#define CODE_GRAPH_H

#include <vector>
#include <iostream>
#include <queue>
#include <bitset>
#include <random>

class Node {
    friend class Graph;

private:
    int id;
    std::vector<Node*> adj;

public:
    Node(int id):id(id) {}
    Node(): Node(0) {}
    void add_edge(Node* to);
    const std::vector<Node*>& get_adjacent() const;

    int get_id() const {
        return id;
    }
};

class Graph {

private:
    friend std::ostream& operator<< (std::ostream& ofs, const Graph& g);
    friend std::istream& operator>> (std::istream& ifs, Graph& g);

    int n;
    bool directed;
    std::vector<Node> nodes;

    void init_nodes(int nodes_reserve) {
        nodes.reserve(n);
        for(int i = 0; i < n; ++i) nodes.emplace_back(i);
    }

public:
    Graph(int n, bool directed=false): n(n), directed(directed) {
        init_nodes(n);
    }
    Graph(bool directed=false): n(0), directed(directed) {}

    bool edge_exists(int u, int v);
    void add_edge(int u, int v);

    std::vector<Node> get_all_nodes() {return nodes;}

    Node& get_node(int u) {
        return nodes[u];
    }

    const Node& get_cnode(int u) {
        return nodes[u];
    }

    int get_total_nodes() const {return n;}

    std::vector<int> get_adjacent(int u) const;

};

class edge_exists_exception: std::exception {
public:
    int u, v;
    edge_exists_exception(int u, int v): u(u), v(v) {}
};

class ColorNode: public Node {

private:
    int color;

public:
    ColorNode(int id, int color): Node(id), color(color) {}
    ColorNode(int id): ColorNode(id, 0) {}

    void set_color(int color) {this -> color = color;}
    int get_color() {return color;}
};

std::ostream& operator<< (std::ostream& ofs, const Graph& g);
std::istream& operator>> (std::istream& ifs, Graph& g);

#endif //CODE_GRAPH_H
