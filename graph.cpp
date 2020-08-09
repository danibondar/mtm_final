#include "graph.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

Graph::Graph(const set<string>& vertices, const set<vector<string>>& edges) {
    map<string, set<string>> graph_map;
    for (auto it = vertices.begin(); it != vertices.end(); it++) {
        set<string> dest;
        for (auto it_2 = edges.begin(); it_2 != edges.end(); it_2++) {
            if ((*it_2)[0] == *it) {
                dest.insert((*it_2)[1]);
            }
        }
        graph_map.insert({*it, dest});
    }
    connections = graph_map;
}

Graph::Graph(const Graph& graph) : connections(graph.connections) {}

Graph& Graph::operator=(const Graph& graph) {
    connections = graph.connections;
    return *this;
}

Graph Graph::operator+(const Graph& graph) const {
    map<string, set<string>> new_map = this->connections;
    for (const auto& connection : graph.connections) {
        auto itr = new_map.find((connection.first));
        if (itr == new_map.end()) {
            new_map.insert({connection.first, connection.second});
        } else {
            (itr->second).insert(connection.second.begin(), connection.second.end());
        }
    }
    return Graph(new_map);
}

Graph Graph::operator^(const Graph& graph) const {
    map<string, set<string>> new_map;
    for (const auto& connection : connections) {
        set<string> dest;
        auto his_vertex = graph.connections.find(connection.first);
        if (his_vertex != graph.connections.end()) {
            for (const auto& itr_vertex : (connection.second)) {
                auto his_vertex_edge = his_vertex->second.find(itr_vertex);
                if (his_vertex_edge != (his_vertex->second).end()) {
                    dest.insert(itr_vertex);
                }
            }
            new_map.insert({connection.first, dest});
        }
    }
    return Graph(new_map);
}

Graph Graph::operator-(const Graph& graph) const {
    map<string, set<string>> new_map;
    for (const auto& connection : connections) {
        set<string> vertices;
        if (graph.connections.find(connection.first) == graph.connections.end()) {
            for (const auto& dest_vertex : connection.second){
                if (graph.connections.find(dest_vertex) == graph.connections.end()){
                    vertices.insert(dest_vertex);
                }
            }
            new_map.insert({connection.first, vertices});
        }
    }
    return Graph(new_map);
}

Graph operator!(const Graph& graph) {
    map<string, set<string>> new_map;
    set<string> vertices;
    for (const auto& connection : graph.connections) {
        vertices.insert(connection.first);
    }
    for (const auto& connection : graph.connections) {
        set<string> new_vertices;
        for (const auto& vertex : vertices) {
            if (connection.second.find(vertex) == connection.second.end() && vertex!= connection.first) {
                new_vertices.insert(vertex);
            }
        }
        new_map.insert({connection.first, new_vertices});
    }
    return Graph(new_map);
}

string make_product_vertex(const string& first_vertex, const string& second_vertex) {
    return "[" + first_vertex + ";" + second_vertex + "]";
}

Graph Graph::operator*(const Graph& graph) const {
    map<string, set<string>> new_map;
    for (const auto& connection : connections) {
        for (const auto& his_connection: graph.connections) {
            set<string> edges;
            string new_vertex = make_product_vertex(connection.first, his_connection.first);
            for (const auto& vertex : connection.second) {
                for (const auto& his_vertex : his_connection.second) {
                    string new_edge = make_product_vertex(vertex, his_vertex);
                    edges.insert(new_edge);
                }
            }
            new_map.insert({new_vertex, edges});
        }
    }
    return Graph(new_map);
}

void Graph::print(ostream& outfile) const {
    for (const auto& connection : connections) {
        outfile << connection.first << endl;
    }
    outfile<<"$"<<endl;
    for (const auto& connection : connections) {
        for (const auto& vertex : connection.second) {
            outfile << connection.first << " " << vertex << endl;
        }
    }
}

Graph::Graph(map<string, set<string>> connection) : connections(connection) {}

