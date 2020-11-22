#include "reader.h"
#include "utilities.h"
#include "Exceptions.h"
#include "evaluator.h"
#include "commends.h"
#define DELETE "delete"
#define WHO "who"
#define RESET "reset"
#define QUIT "quit"
#define PRINT "print"
#define SAVE "save"
#define LOAD "load"
#define FUNCTION "function"
void reader::read(const vector<shared_ptr<Token>>& token_vector) {
    try {
        if (!token_vector.empty()) {
            if ((*token_vector[0]).get_type() == FUNCTION) {
                save_function(token_vector);
            } else {
                string graph_name = (*token_vector[0]).get_name();
                if ((*token_vector[1]).get_name() != "=" || !utilities::is_graph(graph_name)) {
                    throw Undefined_syntax();
                }
                evaluator eval(token_vector, token_vector.cbegin() += 2);
                shared_ptr<Graph> graph = eval.evaluate();
                graph_map[graph_name] = graph;
            }
        }
    } catch (...) { throw; }
}

void reader::save_function(const vector<shared_ptr<Token>>& token_vector) {
    try {
        commends fun(token_vector, graph_map,
                 outfile);
        string first_token = (token_vector[0])->get_name();
        if (first_token == WHO) {
            fun.who();
        } else if (first_token == RESET) {
            fun.restart();
        } else if (first_token == QUIT) {
            fun.quit();
        } else if (first_token == DELETE) {
            fun.delete_graph();
        } else if (first_token == PRINT) {
            fun.print();
        } else if (first_token == SAVE) {
            fun.save();
        }
    } catch (...) { throw; }
}
