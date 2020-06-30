//
// Created by apple on 2020/6/21.
//

#ifndef CODE_GRAPHMAKER_H
#define CODE_GRAPHMAKER_H

#include <utility>

#include "Graph.h"

class GraphMaker {

private:
    Graph super_graph;
    int n_node_each_part;

public:
    GraphMaker(Graph super_graph, int n_node_each_part): super_graph(std::move(super_graph)), n_node_each_part(n_node_each_part) {}
    Graph make();

};


#endif //CODE_GRAPHMAKER_H
