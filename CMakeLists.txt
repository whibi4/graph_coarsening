cmake_minimum_required(VERSION 3.16)

project(graph_coarsening LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/graph.cpp
    src/coarsener.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
