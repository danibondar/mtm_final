#ifndef FINAL_PROJECT_TOKENS_H
#define FINAL_PROJECT_TOKENS_H

#include <iostream>
#include <memory>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "graph.h"

using std::string;
using std::vector;
using std::set;
using std::map;

class Token {
private:

public:
    string type;
    string name = nullptr;
    std::shared_ptr<Graph> ptr = nullptr;
    Token (string type,string name,std::shared_ptr<Graph>ptr):type(type),name(name),ptr(ptr){};
};

#endif //FINAL_PROJECT_TOKENS_H
