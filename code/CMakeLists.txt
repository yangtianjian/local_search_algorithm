cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 11)

add_executable(code Graph.cpp Graph.h GraphMaker.cpp GraphMaker.h Solver.cpp Solver.h test.cpp)