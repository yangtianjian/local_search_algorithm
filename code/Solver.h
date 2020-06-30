//
// Created by apple on 2020/6/21.
//

#ifndef CODE_SOLVER_H
#define CODE_SOLVER_H

#include "Graph.h"
#include <utility>
#include <vector>
#include <utility>
#include <string>
#include <functional>
#include <cmath>
#include <set>
#include <thread>
#include <exception>

class TLEException: public std::exception {
public:
    const char* what() const _NOEXCEPT final {
        return "Time limit exceed";
    }
};


class Result {
private:
    std::vector<int> colors;
    bool has_ans;
    std::string message;

public:

    Result(int n_nodes): colors(n_nodes, -1), has_ans(true) {}
    Result(): Result(0) {}

    const std::vector<int>& get_colors_c() const {return colors;}
    std::vector<int>& get_colors() {return colors;}
    int& operator[] (int i) {return colors[i];}
    Result& operator= (const Result& another) = default;
    void set_answerable(bool answerable) {has_ans = answerable;}
    void set_message(const std::string& msg) {message = msg;}
    std::string get_message() {return message;}
    bool answerable() { return has_ans;}

    int target_function(const Graph& g);  // the cost function for the result on graph g, small is better

    Result& operator |= (Result& another) {
        for(int i = 0; i < colors.size(); ++i) {
            if(colors[i] == -1) colors[i] = another.colors[i];
        }
        return *this;
    }
};

class Solver {
protected:
    void uniform_initialization(Result &result, int total_nodes, int max_color) const;

    void tle_checkpoint(clock_t start_time) const {
        if((clock() - start_time) / CLOCKS_PER_SEC >= 5) {
            throw TLEException();
        }
    }
public:
    virtual Result solve(const Graph& g, int max_color) const = 0;

};

class SystematicSearchSolver: public Solver {


private:
    bool dfs(const Graph& g, int dep, int u, int k, Result& cur_option, clock_t start_time) const;

public:
    Result solve(const Graph& g, int max_color) const;
};


class GreedySearchSolver: public Solver {
private:
    int max_iter;

public:
    GreedySearchSolver(int max_iter): max_iter(max_iter) {}
    GreedySearchSolver(): GreedySearchSolver(-1) {}

    Result solve(const Graph& g, int max_color) const override;
};


class SimulateAnnealingSolver: public Solver {
private:
    int max_iter;

public:
    SimulateAnnealingSolver(int max_iter): max_iter(max_iter)  {}
    Result solve(const Graph& g, int max_color) const override;
};


#endif //CODE_SOLVER_H
