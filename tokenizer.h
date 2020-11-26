#ifndef FINAL_PROJECT_TOKENIZER_H
#define FINAL_PROJECT_TOKENIZER_H

#include <cstdlib>
#include "tokens.h"

using namespace std;

class tokenizer {
private:
    map<string, shared_ptr<Graph>> graph_map;
    shared_ptr<Graph> graph_builder(const vector<string>& vector_string);
public:
    explicit tokenizer( map<string, shared_ptr<Graph>> graph_map):graph_map(std::move(graph_map)){}
    vector<shared_ptr<Token>> to_token(const vector<string>& vector_string);
};

#endif //FINAL_PROJECT_TOKENIZER_H
