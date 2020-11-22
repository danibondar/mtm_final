#include "evaluator.h"
#include "utilities.h"
#define IDENTIFIER "identifier"

bool evaluator::is_over() {
    auto next_token = next(it);
    if (next_token - token_vector.end() == 0) {
        return true;
    } else if ((*next_token)->get_name() == ")") {
        auto next_token_2 = next(next_token, 1);
        if (next_token_2 - token_vector.end() == 0) {
            return true;
        } else {
            it++;
            return true;
        }
    } else return false;
}

void evaluator::argument_finder(shared_ptr<Graph>& arg) {
    try {
        if ((*it)->get_type() == IDENTIFIER) {
            arg = (*it)->get_ptr();
        } else if ((*it)->get_name() == "(") {
            it++;
            arg = evaluate();
        } else if ((*it)->get_name() == "!" && it != token_vector.end()) {
            it++;
            shared_ptr<Graph> temp(new Graph(!(*evaluate())));
            arg = temp;
        } else {
            throw Undefined_syntax();
        }
    } catch (...) { throw; }
}

shared_ptr<Graph> evaluator::evaluate(shared_ptr<Graph> first_arg) {
    string operator_symbol;
    shared_ptr<Graph> second_arg;
    try {
        if (first_arg == nullptr) {
            argument_finder(first_arg);
            if (is_over()) {
                return first_arg;
            }
            it++;
            if (utilities::is_operator((*it)->get_name())) {
                operator_symbol = (*it)->get_name();
                it++;
            } else {
                throw Undefined_syntax();
            }
            argument_finder(second_arg);
            if (is_over()) {
                return math(first_arg, operator_symbol, second_arg);
            } else {
                return evaluate(math(first_arg, operator_symbol, second_arg));
            }
        } else {
            it++;
            if (utilities::is_operator((*it)->get_name())) {
                if (it == (token_vector.end() - 1)) {
                    throw Undefined_syntax();
                }
                operator_symbol = (*it)->get_name();
                it++;
            } else {
                throw Undefined_syntax();
            }

            argument_finder(second_arg);
            if (is_over()) {
                return math(first_arg, operator_symbol, second_arg);
            } else {
                return evaluate(math(first_arg, operator_symbol, second_arg));
            }
        }
    } catch (...) { throw; }

}

shared_ptr<Graph> evaluator::math(const shared_ptr<Graph>& first_arg, string& operator_symbol,
                                  const shared_ptr<Graph>& second_arg) {
    try {
        if (first_arg == nullptr || second_arg == nullptr) {
            throw Undefined_variable();
        }
        else if (operator_symbol == "+") {
            shared_ptr<Graph> res(new Graph((*first_arg) + (*second_arg)));
            return res;
        }
        else if (operator_symbol == "^") {
            shared_ptr<Graph> res(new Graph((*first_arg) ^ (*second_arg)));
            return res;
        }
        else if (operator_symbol == "-") {
            shared_ptr<Graph> res(new Graph((*first_arg) - (*second_arg)));
            return res;
        }
        else if (operator_symbol == "*") {
            shared_ptr<Graph> res(new Graph((*first_arg) * (*second_arg)));
            return res;
        }
    } catch (...) { throw; }
    return shared_ptr<Graph>();
}

evaluator::evaluator(const vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token> >::const_iterator& it)
        : token_vector(token_vector), it(it) {}


