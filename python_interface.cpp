#include "python_interface.h"
#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
#include <fstream>
#include "graph.h"
#include "cheack_functions.h"
#define ERROR "Error: "


Graph* create() {
    Graph* g;
    try {
        g = new Graph();
    } catch (const std::bad_alloc& e) {
        std::cout << ERROR << "bad alloc" << std::endl;
        return nullptr;
    }
    return g;
}

void destroy(Graph* graph) {
    if (graph == nullptr) {
        std::cout << ERROR << "null input" << std::endl;
    }
    delete graph;
}

Graph* addVertex(Graph* graph, const string& vertex) {
    try {
        if (graph == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            return nullptr;
        }
        if (!is_vertex(vertex)) {
            std::cout << ERROR << "not a vertex" << std::endl;
            return nullptr;
        }
        if (*graph->connections.find(vertex) == *graph->connections.end()) {
            std::cout << ERROR << "vertex already exists" << std::endl;
            return nullptr;
        }
        (*graph).connections[vertex];
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph;
}

Graph* addEdge(Graph* graph, const string& first_edge, const string& second_edge) {
    try {
        if (graph == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            return nullptr;
        }
        if (*graph->connections.find(first_edge) == *graph->connections.end() ||
            *graph->connections.find(second_edge) == *graph->connections.end()) {
            std::cout << ERROR << "vertex does not exists" << std::endl;
            return nullptr;
        }
        if (first_edge == second_edge) {
            std::cout << ERROR << "no self loops" << std::endl;
            return nullptr;
        }
        for (auto& dest :graph->connections[first_edge]) {
            if (dest == second_edge) {
                std::cout << ERROR << "no parallel loops" << std::endl;
                return nullptr;
            }
        }
        graph->connections[first_edge].insert(second_edge);
    }
    catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph;
}

void disp(const Graph* graph) {
    try {
        if (graph == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
        }
        (*graph).print(std::cout);
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
    }
}

Graph* graphUnion(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out) {
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            graph_out = nullptr;
        }
        *graph_out = *graph_in1 + *graph_in2;
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph_out;
}

Graph* graphIntersection(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out) {
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            graph_out = nullptr;
        }
        *graph_out = *graph_in1 ^ *graph_in2;
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph_out;
}

Graph* graphDifference(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out) {
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            graph_out = nullptr;
        }
        *graph_out = *graph_in1 - *graph_in2;
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph_out;
}

Graph* graphProduct(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out) {
    try {
        if (graph_in1 == nullptr || graph_in2 == nullptr || graph_out == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            graph_out = nullptr;
        }
        *graph_out = *graph_in1 * *graph_in2;
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph_out;
}

Graph* graphComplement(const Graph* graph_in, Graph* graph_out) {
    try {
        if (graph_in == nullptr || graph_out == nullptr) {
            std::cout << ERROR << "null input" << std::endl;
            graph_out = nullptr;
        }
        *graph_out = !*graph_in;
    } catch (...) {
        std::cout << ERROR << "invalid input" << std::endl;
        return nullptr;
    }
    return graph_out;
}
