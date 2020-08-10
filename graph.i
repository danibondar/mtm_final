%module graph
%{
#include "python_interface.h"
%}



Graph* create();
void destroy(Graph* graph);
Graph* addVertex(Graph* graph, const string
& vertex);
Graph* addEdge(Graph* graph, const string
& first_edge, const string& second_edge);
void disp(const Graph* graph);
Graph* graphUnion(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out);
Graph* graphIntersection(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out);
Graph* graphDifference(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out);
Graph* graphProduct(const Graph* graph_in1, const Graph* graph_in2, Graph* graph_out);
Graph* graphComplement(const Graph* graph_in, Graph* graph_out);