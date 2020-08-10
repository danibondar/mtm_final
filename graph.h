#ifndef FINAL_PROJECT_GRAPH_H
#define FINAL_PROJECT_GRAPH_H

#include <iostream>
#include <memory>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <set>
#include <map>

using std::string;
using std::vector;
using std::set;
using std::map;

class Graph {
private:

public:
    map<string, set<string> > connections;
    Graph(const set<string>& vertices,const set<vector<string> >& edges);
    Graph(map<string, set<string> > connection);
    ~Graph() = default;
    Graph(const Graph& graph);
    Graph& operator=(const Graph& graph);
    Graph operator+(const Graph& graph) const;
    Graph operator^(const Graph& graph) const;
    Graph operator-(const Graph& graph) const;
    Graph operator*(const Graph& graph) const;
    void print(std::ostream& outfile)const ;
    Graph() = default;
};
Graph operator!(const Graph& graph);


#endif //FINAL_PROJECT_GRAPH_H
