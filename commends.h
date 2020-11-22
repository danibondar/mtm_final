#ifndef FINAL_PROJECT_COMMENDS_H
#define FINAL_PROJECT_COMMENDS_H

#include "tokens.h"

using namespace std;

class commends {
private:
    const vector<shared_ptr<Token>> token_vector;
    map<string, shared_ptr<Graph>> graph_map;
    ostream& outfile;
    void write_to_file(const shared_ptr<Graph>& graph, const shared_ptr<Token>& file_name);
public:
    commends(const vector<shared_ptr<Token>>& token_vector, map<string, shared_ptr<Graph>>& graph_map,
            ostream& outfile);
    void who();
    void print();
    void save();
    void restart();
    void quit();
    void delete_graph();
};

#endif //FINAL_PROJECT_COMMENDS_H
