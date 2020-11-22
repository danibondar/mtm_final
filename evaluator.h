#ifndef FINAL_PROJECT_EVALUATOR_H
#define FINAL_PROJECT_EVALUATOR_H
#include <cstdlib>
#include "graph.h"
#include "tokens.h"
#include "Exceptions.h"

class evaluator {
public:
    const vector<shared_ptr<Token> >& token_vector;
    vector<shared_ptr<Token> >::const_iterator& it;
    void argument_finder(shared_ptr<Graph>& arg);
    bool is_over();
    shared_ptr<Graph> math(const shared_ptr<Graph>& first_arg, string& operator_symbol,
                           const shared_ptr<Graph>& second_arg);

public:
    evaluator(const vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token> >::const_iterator& it);
    shared_ptr<Graph> evaluate(shared_ptr<Graph> first_arg = nullptr);
};

#endif //FINAL_PROJECT_EVALUATOR_H
