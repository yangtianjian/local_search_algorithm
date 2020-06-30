//
// Created by apple on 2020/6/21.
//

#include "GraphMaker.h"

Graph GraphMaker::make() {
    std::vector<Node> super_nodes = super_graph.get_all_nodes();
    Graph g((int)super_nodes.size() * n_node_each_part, false);
    // all nodes in part k: [k * n_node_each_part: (k + 1) * n_node_each_part]

    std::default_random_engine engine;

    std::uniform_real_distribution<int> distribution;

    for(const Node& u: super_nodes) {
        int uid = u.get_id();
        auto &adj = u.get_adjacent();
        for(const Node* v: adj) {
            int vid = v -> get_id();
            if(uid < vid) {
                for(int u2 = 0; u2 < n_node_each_part; ++u2) {
                    for(int v2 = 0; v2 < n_node_each_part; ++v2) {
                        double r = distribution(engine);
                        if(r < 0.8) {
                            g.add_edge(uid * n_node_each_part + u2, vid * n_node_each_part + v2);
                        }
                    }
                }
            }
        }
    }
    return g;
}