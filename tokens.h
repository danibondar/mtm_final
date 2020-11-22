#ifndef FINAL_PROJECT_TOKENS_H
#define FINAL_PROJECT_TOKENS_H

#include <cstdlib>
#include "graph.h"

using namespace std;

class Token {
private:
    string type;
    string name = "";
    shared_ptr<Graph> ptr = nullptr;

public:
    string get_type() { return type; }

    string get_name() { return name; }

    shared_ptr<Graph> get_ptr() { return ptr; }

    Token(string type, string name, shared_ptr<Graph> ptr) : type(move(type)), name(move(name)), ptr(move(ptr)) {};
};

#endif //FINAL_PROJECT_TOKENS_H
