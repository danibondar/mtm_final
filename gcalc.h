#ifndef FINAL_PROJECT_GCALC_H
#define FINAL_PROJECT_GCALC_H

#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>
#include <vector>
#include <set>
#include <fstream>
#include "graph.h"
#include "tokens.h"
#include "Exceptions.h"

std::shared_ptr<Graph>
evaluate(vector<std::shared_ptr<Token>>& token_vector, vector<std::shared_ptr<Token>>::iterator& it,
         std::shared_ptr<Graph> first_arg = nullptr);
bool is_vertex(const string& str);
bool is_graph(const string& str);
void syntax_checker(vector<string>& output_vector);
std::shared_ptr<Graph>
math(std::shared_ptr<Graph>& first_arg, string& operator_symbol, std::shared_ptr<Graph>& second_arg);

#endif //FINAL_PROJECT_GCALC_H
