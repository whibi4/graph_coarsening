#include "graph.hpp"
#include <iostream>
#include <chrono>
int main() {
    const char* dataset_name = std::getenv("DATASET_PATH");
    if (dataset_name == nullptr) {
        std::cerr << "[ERR]: DATASET_PATH is not set\n";
        return 1;
    } else {
        std::cout << "[INFO]: DATASET_PATH is set to \""<<dataset_name<<"\"\n";
    }
    Graph *graph;
    {
        auto t0 = std::chrono::steady_clock::now();
        graph = Graph::createFromDataset(dataset_name);
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "[INFO]: Graph created in " << std::chrono::duration<double>(t1 - t0).count() << "s\n";
    }
    {
        auto t0 = std::chrono::steady_clock::now();
        graph->printGraphStats();
        auto t1 = std::chrono::steady_clock::now();
        std::cout << "[INFO]: Graph stats printed in " << std::chrono::duration<double>(t1 - t0).count() << "s\n";
    }
}