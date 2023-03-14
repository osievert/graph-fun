cmake_minimum_required(VERSION 3.18.0)

project(graphs)

add_executable(graphs
    src/main.cpp)

set_target_properties(graphs PROPERTIES CXX_STANDARD 17)
