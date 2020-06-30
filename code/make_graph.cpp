#include <iostream>
#include <fstream>
#include <vector>
#include "Graph.h"
#include "GraphMaker.h"
#include <ios>
#include <sstream>
#include <cstdlib>
#include <random>

using namespace std;

void generate_data() {
    vector<int> n_nodes_for_each_part{10, 50, 100};

    for(int i = 0; i < 3; ++i) {
        ostringstream ssi;
        ssi << "/Users/apple/Desktop/homework/advanced_algorithm/code/data/";
        ssi << "example" << i + 1 << ".txt";
        fstream fin(ssi.str(), fstream::in);
        Graph g_super(false);
        fin >> g_super;
        fin.close();

        for(int j = 0; j < n_nodes_for_each_part.size(); ++j) {
            ostringstream ss;
            ss << "/Users/apple/Desktop/homework/advanced_algorithm/code/data/";
            ss << i + 1 << "_" << j + 1 << ".txt";
            GraphMaker gm(g_super, n_nodes_for_each_part[j]);
            Graph g_out = gm.make();
            fstream fout(ss.str(), fstream::out);
            fout << g_out << std::flush;
            fout.close();
        }
    }
}

int main() {
    generate_data();

    return 0;
}