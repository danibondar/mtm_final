#include <iostream>
#include <memory>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "graph.h"
#include "tokens.h"

#define DELETE "delete"
#define WHO "who"
#define RESET "reset"
#define QUIT "quit"
#define PRINT "print"
#define FUNCTION "function"
#define REGEX "regex"
#define GRAPH "graph"
using namespace std;

shared_ptr<Graph> evaluate(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                           shared_ptr<Graph> first_arg = nullptr);

bool is_symbol(const char val) {
    if (val < '0' || (val > '9' && val < 'A') || (val > 'Z' && val < 'a') || val > 'z') {
        if (val != '[' && val != ']' && val != ';') {
            return true;
        }
    }
    return false;
}

bool is_vertex(const string& str) {
    int square_bracket = 0;
    for (char it : str) {
        if (is_symbol(it)) {
            return false;
        }
        if (it == '[') {
            square_bracket++;
        }
        if (it == ']') {
            if (square_bracket <= 0) {
                return false;
            } else { square_bracket--; }
        }
        if (it == ';') {
            if (square_bracket <= 0) {
                return false;
            }
        }
    }
    if (square_bracket != 0) {
        return false;
    }
    return !(str == DELETE || str == PRINT || str == WHO || str == QUIT || str == RESET);

}

bool is_graph(const string& str) {
    if (str == DELETE || str == PRINT || str == WHO || str == QUIT || str == RESET) {
        return false;
    }
    if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z')) {
        for (char it : str) {
            if (it < '0' || (it > '9' && it < 'A') || (it > 'Z' && it < 'a') || it > 'z') {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool is_operator(const string& str) {
    return str == "+" || str == "^" || str == "*" || str == "-";
}

vector<string> parse_to_vector(string& str) {
    vector<string> input_stack;
    string temp_str;
    for (char& it : str) {
        if (it == ' ' and !temp_str.empty()) {
            input_stack.push_back(temp_str);
            temp_str = "";
        }
        if (is_symbol(it)) {
            if (!temp_str.empty()) {
                input_stack.push_back(temp_str);
                temp_str = it;
                input_stack.push_back(temp_str);
                temp_str = "";
            } else {
                temp_str = it;
                input_stack.push_back(temp_str);
                temp_str = "";
            }
        } else {
            //not a symbol
            temp_str.push_back(it);
        }
    }
    if (!temp_str.empty()) {
        input_stack.push_back(temp_str);
    }
    return input_stack;
}

void saved_function_checker(vector<string>& output_vector) {
    bool saved_function = false;
    for (auto it = output_vector.begin(); it != output_vector.end(); it++) {
        if (it == output_vector.begin()) {
            if (*it == DELETE || *it == PRINT || *it == WHO || *it == QUIT || *it == RESET) {
                saved_function = true;
                if (*it == WHO || *it == QUIT || *it == RESET) {
                    if (output_vector.size() != 1) {
                        //todo:: exception wrong syntax
                    }
                } else if (*it == DELETE) {
                    if ((*(it++) != "(") || (!is_graph(*(it++))) || (*(it++) != ")") || output_vector.size() != 4) {
                        //todo:: exception wrong syntax
                    }
                } else {
                    auto next_token = next(it, 1);
                    if (!(*next_token == "(" && *output_vector.end() == ")") || output_vector.size() == 3) {
                        //todo:: exception wrong syntax
                    }
                }
            } else {
                break;
            }
        } else if (saved_function &&
                   (*it == DELETE || *it == PRINT || *it == WHO || *it == QUIT || *it == RESET)) {
            //todo:: exception wrong syntax
        }
    }
}

void equal_symbol_checker(vector<string>& output_vector) {
    int counter = 0;
    for (auto& it : output_vector) {
        if (it == "=") {
            counter++;
        }
    }
    if (output_vector[1] == "=") {
        if (counter != 1 || !is_graph(output_vector[0])) {
            //todo:: exception wrong syntax
        }
    } else {
        if (counter != 0 || (output_vector[0] != DELETE && output_vector[0] != PRINT && output_vector[0] != WHO
                             && output_vector[0] != QUIT && output_vector[0] != RESET)) {
            //todo:: exception wrong syntax
        }
    }
}

void even_brackets(vector<string>& output_vector) {
    int round_bracket = 0;
    int curly_bracket = 0;
    int pipeline = 0;
    int greater_than_sign = 0;
    for (auto& it : output_vector) {
        if (it == "(") {
            if (curly_bracket == 0) {
                round_bracket++;
            } else {
                //todo:: exception wrong syntax
            }
        }
        if (it == ")") {
            if (curly_bracket == 0 && round_bracket > 0) {
                round_bracket--;
            } else {
                //todo:: exception wrong syntax
            }
        }
        if (it == "{") {
            if (curly_bracket != 0) {
                //todo:: exception wrong syntax
            } else {
                curly_bracket++;
            }
        }
        if (it == "|") {
            if (curly_bracket != 1 || pipeline != 0) {
                //todo:: exception wrong syntax
            } else {
                pipeline++;
            }
        }
        if (it == "<") {
            if (curly_bracket != 1 || greater_than_sign != 0) {
                //todo:: exception wrong syntax
            } else {
                greater_than_sign++;
            }
        }
        if (it == ">") {
            if (curly_bracket != 1 || greater_than_sign != 1) {
                //todo:: exception wrong syntax
            } else {
                greater_than_sign--;
            }
        }
        if (it == "}") {
            if (curly_bracket != 1 || pipeline != 1) {
                //todo:: exception wrong syntax
            } else {
                curly_bracket--;
                pipeline--;
            }
        }
    }
    if (curly_bracket != 0 || pipeline != 0 || round_bracket != 0 || greater_than_sign != 0) {
        //todo:: exception wrong syntax
    }
}

void graph_syntax_checker(vector<string>& output_vector) {
    bool pipline = false;
    bool in_brcket = false;

    for (auto it = output_vector.begin(); it != output_vector.end(); it++) {
        auto next_token = next(it, 1);
        if (*it == "{") {
            if (!(*next_token == "}")) {
                in_brcket = true;
            } else {
                it += 2;
            }
        } else {
            if (in_brcket && !pipline) {
                if (is_vertex(*it)) {
                    if (*next_token == ",") {
                        it++;
                    } else if (*next_token == "|") {
                        it++;
                        pipline = true;
                    } else if (*next_token == "}") {
                        it++;
                        in_brcket = false;
                    } else {
                        //todo:: exception wrong syntax
                    }
                } else {
                    //todo:: exception wrong syntax
                }
            }
            if (in_brcket && pipline) {
                if (*it == "<") {
                    it++;
                    if (is_vertex(*it)) {
                        it++;
                        if (*it == ",") {
                            it++;
                            if (is_vertex(*it)) {
                                it++;
                                if (*it == ">") {
                                    it++;
                                    if (*it == "}") {
                                        it++;
                                        in_brcket = false;
                                    } else if (*it == ",") {
                                        it++;
                                    } else {
                                        //todo:: exception wrong syntax
                                    }
                                } else {
                                    //todo:: exception wrong syntax
                                }
                            } else {
                                //todo:: exception wrong syntax
                            }
                        } else {
                            //todo:: exception wrong syntax
                        }
                    } else {
                        //todo:: exception wrong syntax
                    }
                } else {
                    //todo:: exception wrong syntax
                }
            }
        }

    }
}

void syntax_checker(vector<string>& output_vector) {
    //TODO::try and catch
    equal_symbol_checker(output_vector);
    saved_function_checker(output_vector);
    even_brackets(output_vector);
    graph_syntax_checker(output_vector);
}

bool is_over(vector<shared_ptr<Token>> token_vector, vector<shared_ptr<Token>>::iterator& it) {
    if ((*it)->name == ")") {
        if (it == token_vector.end()) {
            return true;
        } else {
            it++;
            return true;
        }
    } else return it == token_vector.end();
}

shared_ptr<Graph> regex_evaluate(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                                 shared_ptr<Graph> first_arg = nullptr) {
    return evaluate(token_vector, it, first_arg);
}

void argument_finder(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                     shared_ptr<Graph>& arg) {
    if (is_graph((*it)->name)) {
        arg = (*it)->ptr;
        it++;
    } else if ((*it)->name == "(") {
        it++;
        arg = regex_evaluate(token_vector, it);
    } else if ((*it)->name == "!") {
        auto next_token = next(it, 1);
        if (is_graph((*next_token)->name)) {
            shared_ptr<Graph> temp(new Graph(!(*(*it)->ptr)));
            arg = temp;
        } else if ((*next_token)->name == "(") {
            arg = regex_evaluate(token_vector, it);
        } else {
            //TODO::exception wrong syntax
        }
    } else {
        //TODO::exception wrong syntax
    }
}

shared_ptr<Graph> math(shared_ptr<Graph>& first_arg, string& operator_symbol, shared_ptr<Graph>& second_arg) {
    shared_ptr<Graph> res_2;
    if (operator_symbol == "+") {
        shared_ptr<Graph> res(new Graph((*first_arg)+(*second_arg)));
    }
    if (operator_symbol == "^") {
        shared_ptr<Graph> res(new Graph((*first_arg)^(*second_arg)));
        return res;
    }
    if (operator_symbol == "-") {
        shared_ptr<Graph> res(new Graph((*first_arg)-(*second_arg)));
        return res;
    }
    if (operator_symbol == "*") {
        shared_ptr<Graph> res(new Graph((*first_arg)*(*second_arg)));
        return res;
    }
    assert("cant reach here");
    return shared_ptr<Graph>();
}

shared_ptr<Graph> evaluate(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                           shared_ptr<Graph> first_arg) {
    if (it == token_vector.end()) {
        return first_arg;
    }
    string operator_symbol;
    shared_ptr<Graph> second_arg;
    if (first_arg == nullptr) {
        auto next_token = next(it);
        if ((*it)->name == "(" && (*next_token)->name == ")") {//todo::is this correct syntax?
            it++;
            is_over(token_vector, it);
            return first_arg;
        }
        next_token = next(it, 2);
        if ((*it)->name == "(" && (*next_token)->name == ")") {
            it++;
            if ((*it)->type == GRAPH) {
                shared_ptr<Graph> temp = (*it)->ptr;
                it++;
                is_over(token_vector, it);
                return temp;
            } else {
                //TODO:: exeption not correct syntext
            }
        }
        next_token = next(it, 3);
        if ((*it)->name == "(" && (*next_token)->name == ")") {
            it++;
            if ((*it)->name == "!" && (*(it++))->type == GRAPH) {
                shared_ptr<Graph> res(new Graph(!(*(*it)->ptr)));
                it++;
                is_over(token_vector, it);
                return res;
            } else {
                //TODO:: exeption not correct syntext
            }
        }
        argument_finder(token_vector, it, first_arg);
        if (is_operator((*it)->name)) {
            operator_symbol = (*it)->name;
            it++;
        } else {
            //TODO:: exeption not correct syntext
        }
        argument_finder(token_vector, it, second_arg);
        if (is_over(token_vector, it)) {
            return math(first_arg, operator_symbol, second_arg);
        } else {
            return evaluate(token_vector, it, math(first_arg, operator_symbol, second_arg));
        }
    } else {
        if (is_operator((*it)->name)) {
            operator_symbol = (*it)->name;
            it++;
        } else {
            //TODO:: exception not correct syntax
        }

        argument_finder(token_vector, it, second_arg);
        if (is_over(token_vector, it)) {
            return math(first_arg, operator_symbol, second_arg);
        } else {
            return evaluate(token_vector, it, math(first_arg, operator_symbol, second_arg));
        }
    }
}

vector<shared_ptr<Token>> tokenizer(vector<string>& output_vector, map<string, shared_ptr<Graph>>& graph_map) {
    vector<shared_ptr<Token>> new_token_vector;
    for (auto token = output_vector.begin(); token != output_vector.end(); token++) {

        if (*token == DELETE || *token == PRINT || *token == WHO || *token == QUIT || *token == RESET) {
            shared_ptr<Token> ptr(new Token(FUNCTION, *token, nullptr));
            new_token_vector.push_back(ptr);

        } else if (*token == "(" || *token == ")" || *token == "+" || *token == "-" || *token == "*" || *token == "^" ||
                   *token == "!" || *token == "=") {
            shared_ptr<Token> ptr(new Token(REGEX, *token, nullptr));
            new_token_vector.push_back(ptr);

        } else if (is_graph(*token)) {
            if ((token - output_vector.begin()) == 0) {
                shared_ptr<Token> ptr(new Token(GRAPH, *token, nullptr));
                new_token_vector.push_back(ptr);
            } else {
                auto it = graph_map.find(*token);
                if (it == graph_map.end()) {
                    //todo::exception graph doesn't exists
                }
                shared_ptr<Token> ptr(new Token(GRAPH, *token, it->second));
                new_token_vector.push_back(ptr);
            }

        } else if (*token == "{") {
            token++;
            set<string> vertices;
            set<vector<string>> edges;
            while (*token != "|" || *token != "}") {
                if (*token == ",") {
                    token++;
                } else {
                    vertices.insert(*token);
                    token++;
                }
            }
            if (*token == "|") {
                while (*token != "}") {
                    vector<string> edge;
                    token++;
                    token++;
                    edge.push_back(*token);
                    token++;
                    token++;
                    edge.push_back(*token);
                    token++;
                    token++;
                    edges.insert(edge);
                }
            }
            if (*token == "}") {
                shared_ptr<Graph> graph_ptr(new Graph(vertices, edges));
                shared_ptr<Token> token_ptr(new Token(GRAPH, *token, graph_ptr));
                new_token_vector.push_back(token_ptr);
            }

        } else {
            assert("wrong syntax");
        }
    }
    return new_token_vector;
}

void read(vector<shared_ptr<Token>>& token_vector, map<string, shared_ptr<Graph>>& graph_map) {
    if (!token_vector.empty()) {
        if ((*token_vector[0]).type == FUNCTION) {
            if ((*token_vector[0]).name == WHO) {
                for (const auto& graph :graph_map) {
                    cout << graph.first << endl;
                }
            } else if ((*token_vector[0]).name == QUIT) {
                exit(0);
            } else if ((*token_vector[0]).name == RESET) {
                graph_map.clear();
            } else if ((*token_vector[0]).name == DELETE) {
                graph_map.erase((*token_vector[3]).name);
            } else if ((*token_vector[0]).name == PRINT) {
                Graph res;
                auto it = token_vector.begin();
                advance(it, 1);
                (*evaluate(token_vector, it)).print();
            }
        } else {

            string graph_name = (*token_vector[0]).name;
            auto it = token_vector.begin();
            advance(it, 2);
            shared_ptr<Graph> graph = evaluate(token_vector, it);
            graph_map.insert({graph_name, graph});
        }
    } else {
        //todo :: check if empty row is correct input
    }
}

int main() {
    map<string, shared_ptr<Graph>> graph_map;
    while (true) {
        string str;
        cout << "Gcalc> ";
        getline(cin, str);
        vector<string> output_vector = parse_to_vector(str);
        syntax_checker(output_vector);//todo:: try catch
        vector<shared_ptr<Token>> token_vector = tokenizer(output_vector, graph_map);
        read(token_vector, graph_map);
    }
    return 0;
}
