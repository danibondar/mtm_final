#ifndef FINAL_PROJECT_GCALC_H
#define FINAL_PROJECT_GCALC_H

#include <cstdlib>
#include "graph.h"
#include "input_stream.h"

using namespace std;

class gcalc {
private:
    map<string, shared_ptr<Graph>> graph_map;
    input_stream stream;
public:
    explicit gcalc(int argc, char**& argv);
    void activate_gcalc();
};

#endif //FINAL_PROJECT_GCALC_H