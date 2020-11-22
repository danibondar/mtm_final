#ifndef FINAL_PROJECT_READER_H
#define FINAL_PROJECT_READER_H

#include <cstdlib>
#include "tokens.h"

using namespace std;

class reader {
private:
    map<string, shared_ptr<Graph>> graph_map;
    ostream& outfile;
    void save_function(const vector<shared_ptr<Token>>& token_vector);
public:
    reader(map<string, shared_ptr<Graph>> graph_map, ostream& outfile):graph_map(move(graph_map)),outfile(outfile){};
    void read(const vector<shared_ptr<Token>>& token_vector);
};


#endif //FINAL_PROJECT_READER_H
