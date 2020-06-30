//
// Created by apple on 2020/6/21.
//

#include "Solver.h"

int Result::target_function(const Graph &g) {
    int val = 0;
    for(int u = 0; u < g.get_total_nodes(); ++u) {
        for(int v: g.get_adjacent(u)) {
            if(u < v) {
                if(colors[u] == colors[v]) ++val;
            }
        }
    }
    return val;
}

void Solver::uniform_initialization(Result &result, int total_nodes, int max_color) const {
    std::uniform_int_distribution<int> distribution(0, max_color - 1);
    std::default_random_engine engine;
    for(int i = 0; i < total_nodes; ++i) result[i] = distribution(engine);
}

Result SystematicSearchSolver::solve(const Graph &g, int max_color) const {
    int n_nodes = g.get_total_nodes();
    Result cur_option(n_nodes);
    clock_t start_time = clock();

    for(int i = 0; i < n_nodes; ++i) {
        tle_checkpoint(start_time);
        if(cur_option[i] == -1) {
            bool res = dfs(g, 0, i, max_color, cur_option, start_time);
            if(!res) {
                cur_option.set_message("no_solution");
                cur_option.set_answerable(false);
                return cur_option;
            }
        }
    }

    cur_option.set_answerable(true);
    cur_option.set_message("solved");

    return cur_option;
}

bool SystematicSearchSolver::dfs(const Graph& g, int dep, int u, int k,
                                 Result& cur_option, clock_t start_time) const {
    /*
     * DFS for graph coloring, at depth dep, at node u, with k colors
     * return: If we can find valid solution, then the function returns true. And the solution is in "cur_option" variable.
     */

    tle_checkpoint(start_time);

    int n = g.get_total_nodes();
    std::vector<bool> color_available(k, true);
    std::vector<int> adj = g.get_adjacent(u);

    for(int v: adj) {
        if(cur_option[v] != -1) color_available[cur_option[v]] = false;
    }

    Result ret(n);
    for(int c = 0; c < k; ++c) {

        tle_checkpoint(start_time);

        if(color_available[c]) {
            cur_option[u] = c;
            bool satisfy = true;
            for(int v: adj) {
                if(cur_option[v] == -1) {
                    bool ans = dfs(g, dep + 1, v, k, cur_option, start_time);
                    if(!ans) {
                        satisfy = false;
                        break;
                    }
                }
            }
            if(satisfy) return true;
            cur_option[u] = -1;
        }
    }
    return false;
}

Result GreedySearchSolver::solve(const Graph &g, int max_color) const {
    int n = g.get_total_nodes();
    Result res(n);
    uniform_initialization(res, n, max_color);

    std::vector<std::vector<int>> count(n, std::vector<int>(max_color, 0));  // count[i][k] the count of color k adjacent to i
    for(int u = 0; u < n; ++u) {
        for(int v: g.get_adjacent(u)) {
            ++count[u][res[v]];
        }
    }

    int iter = max_iter;
    bool is_global_opt = false;

    while(iter == -1 || (iter-- > 0)) {   // iter = -1: always iter until local minima

        std::pair<int, int> best_opt{-1, 0};
        int best_value = 0;  // the contradiction that has been reduced

        bool contradict = false;  // has contradiction for each edge globally
        for(int u = 0; u < n; ++u) {
            int current_contradiction = count[u][res[u]];
            if(current_contradiction > 0) contradict = true;
            for(int k = 0; k < max_color; ++k) {
                if(k != res[u]) {  // change one color
                    int changed_contradiction = count[u][k];

                    int evaluate_fn = current_contradiction - changed_contradiction;
                    if(evaluate_fn > best_value) {
                        best_opt.first = u;
                        best_opt.second = k;
                        best_value = evaluate_fn;
                    }
                }
            }
        }
        if(!contradict) {
            is_global_opt = true;
            res.set_message("solved");
            break;  // global optima
        }
        if(best_opt.first == -1) {   // local optima
            res.set_message("local_optima_reached");
            break;
        }
        if(iter == 0) res.set_message("max_iteration_reached");

        // now apply the change, for the next iteration
        int u = best_opt.first, k = best_opt.second;
        int cur_color = res[u];
        for(int v: g.get_adjacent(u)) {
            --count[v][cur_color];
            ++count[v][k];
        }
        res[u] = k;

//        std::cout << "iter: " << max_iter - iter << " cost function: " << res.target_function(g) << std::endl;
    }
    res.set_answerable(is_global_opt);
    return res;
}

Result SimulateAnnealingSolver::solve(const Graph& g, int max_color) const {
    int n = g.get_total_nodes();
    Result res(n);

    std::set<int> node_not_settled;   // The nodes that has contradiction with neighbors

    uniform_initialization(res, n, max_color);
    std::vector<std::vector<int>> count(n, std::vector<int>(max_color, 0));  // count[i][k] the count of color k adjacent to i


    for(int u = 0; u < n; ++u) {
        for(int v: g.get_adjacent(u)) {
            ++count[u][res[v]];
        }
        if(count[u][res[u]] > 0) {
            node_not_settled.insert(u);
        }
    }

    std::default_random_engine e1, e2, e3;
    std::uniform_int_distribution<int> random_choose_node(0, n - 1);
    std::uniform_int_distribution<int> random_choose_color(0, max_color - 1);
    std::uniform_real_distribution<double> random_accept_worse(0.0, 1.0);

    bool global_optima = false;

    int t;
    double T = 100.0;
    double decay = 0.9997;
    for(t = 0; t < max_iter; ++t) {
        if(node_not_settled.empty()) {
            global_optima = true;
            break;
        }

        int u = random_choose_node(e1);
        int k = res[u];
        while(k == res[u]) {
            k = random_choose_color(e2);
        }

        int evaluate_fn = count[u][res[u]] - count[u][k];  // worse: < 0

        double p = exp(evaluate_fn / T);
        double r = random_accept_worse(e3);
        if(evaluate_fn > 0 || r < p) {   // has improvement or just accept the change
            int ori_u = count[u][res[u]];
            for(int v: g.get_adjacent(u)) {
                int ori_v = count[v][res[v]];
                --count[v][res[u]];
                ++count[v][k];
                if(ori_v > 0 && count[v][res[v]] == 0) node_not_settled.erase(v);
                if(ori_v == 0 && count[v][res[v]] > 0) node_not_settled.insert(v);  // for set, neglect the repeated value
            }
            if(ori_u > 0 && count[u][k] == 0) node_not_settled.erase(u);
            if(ori_u == 0 && count[u][k] > 0) node_not_settled.insert(u);
            res[u] = k;
        }

        T = decay * T;
    }

    if(global_optima) {
        res.set_answerable(true);
        res.set_message("solved");
    } else {
        res.set_answerable(false);
        if(t == max_iter) {
            res.set_message("max_iteration_reached");
        }
    }
    return res;
}