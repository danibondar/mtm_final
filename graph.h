#ifndef FINAL_PROJECT_GRAPH_H
#define FINAL_PROJECT_GRAPH_H

#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Graph {
private:
    map<string, set<string>> connections;
public:
    explicit Graph(map<string, set<string>> graph_map);
    ~Graph() = default;
    Graph(const Graph& graph);

    map<string, set<string>> get_connections() const { return connections; };
    Graph& operator=(const Graph& graph);
    Graph operator+(const Graph& graph) const;
    Graph operator^(const Graph& graph) const;
    Graph operator-(const Graph& graph) const;
    Graph operator*(const Graph& graph) const;
    void print(std::ostream& outfile) const;
    Graph() = default;
};

Graph operator!(const Graph& graph);


#endif //FINAL_PROJECT_GRAPH_H
