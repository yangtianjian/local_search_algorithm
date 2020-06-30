//
// Created by apple on 2020/6/21.
//

#include "Graph.h"
#include "Solver.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <memory>

struct BenchmarkResult {
    std::string setting;
    bool answerable;
    int cost_function;
    double time;
    std::string message;

    std::string to_string() {
        std::stringstream ss;
        ss << answerable << "," << message << "," << cost_function << "," << time;
        return ss.str();
    }
};

BenchmarkResult benchmark(Solver* solver, const Graph& g, int max_color, const std::string& setting) {
    BenchmarkResult br;
    clock_t t1 = clock();
    try {
        Result r = solver -> solve(g, max_color);
        br.time = (clock() - t1) / (double)CLOCKS_PER_SEC;
        br.answerable = r.answerable();
        br.cost_function = r.target_function(g);
        br.message = r.get_message();
        br.setting = setting;
    } catch (const TLEException& exception) {
        br.answerable = false;
        br.time = 5;
        br.message = "time_limit_exceed";
        br.cost_function = -1;
        br.setting = setting;
    }
    return br;
}

int main() {

    std::vector<std::string> graph_file_names;
    std::vector<int> max_color = {2, 3, 4, 5, 6};
    std::vector<int> answer = {0, 2, 3, 5};

    auto ps1 = std::shared_ptr<Solver>(new SystematicSearchSolver);
    auto ps2 = std::shared_ptr<Solver>(new GreedySearchSolver(10000));
    auto ps3 = std::shared_ptr<Solver>(new SimulateAnnealingSolver(100000));
    std::vector<std::shared_ptr<Solver>> solvers {ps1, ps2, ps3};
    std::vector<std::string> solver_names{"systematic_search", "greedy_search", "SA"};

    std::fstream result_out("/Users/apple/Desktop/homework/advanced_algorithm/code/data/result.txt", std::fstream::out);
    result_out << "graph,max_color,answer,has_answer,method,solved,message,cost_function,time" << std::endl;
    for(int i = 1; i <= 3; ++i) {
        for(int j = 1; j <= 3; ++j) {
            std::stringstream ss;
            ss << "/Users/apple/Desktop/homework/advanced_algorithm/code/data/";
            ss << i << '_' << j << ".txt";

            std::fstream fs(ss.str(), std::fstream::in);

            Graph g; fs >> g;

            for(int mc: max_color) {
                int k = 0;
                for(const auto& solver: solvers) {
                    std::stringstream setting_ss;
                    setting_ss << i << '_' << j << ",";
                    setting_ss << mc << ",";
                    setting_ss << answer[i] << ",";
                    setting_ss << (mc >= answer[i]) << ",";
                    setting_ss << solver_names[k++];

                    BenchmarkResult res = benchmark(solver.get(), g, mc, setting_ss.str());
                    result_out << setting_ss.str() << "," << res.to_string() << std::endl;
                }
            }

            fs.close();
        }
    }
    result_out.close();

    return 0;
}