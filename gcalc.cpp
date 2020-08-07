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
    try {
        for (char& it : str) {
            if (it == ' ') {
            } else if (is_symbol(it)) {
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
    } catch (...) { throw; }
    return input_stack;
}

void equal_symbol_checker(vector<string>& output_vector) {
    int counter = 0;
    try {
        for (auto& it : output_vector) {
            if (it == "=") {
                counter++;
            }
        }
        if (output_vector.size() > 1) {
            if (output_vector[1] == "=") {
                if (counter != 1 || !is_graph(output_vector[0]) || *(output_vector.end() - 1) == "=") {
                    throw Undefined_syntax();
                }
            } else {
                if (counter != 0 || (output_vector[0] != DELETE && output_vector[0] != PRINT && output_vector[0] != WHO
                                     && output_vector[0] != QUIT && output_vector[0] != RESET)) {
                    throw Undefined_syntax();
                }
            }
        } else if ((output_vector[0] != RESET && output_vector[0] != QUIT && output_vector[0] != WHO) || counter != 0) {
            throw Undefined_syntax();
        }
    } catch (...) { throw; }
}

void saved_function_checker(vector<string>& output_vector) {
    bool saved_function = false;
    try {
        for (auto it = output_vector.begin(); it != output_vector.end(); it++) {
            if (it == output_vector.begin()) {
                if (*it == DELETE || *it == PRINT || *it == WHO || *it == QUIT || *it == RESET) {
                    saved_function = true;
                    if (*it == WHO || *it == QUIT || *it == RESET) {
                        if (output_vector.size() != 1) {
                            throw Undefined_syntax();
                        }
                    } else if (*it == DELETE) {
                        if ((*(next(it)) != "(") || (!is_graph(*(next(it, 2)))) || (*(next(it, 3)) != ")") ||
                            output_vector.size() != 4) {
                            throw Undefined_syntax();
                        }
                    } else {
                        auto next_token = next(it, 1);
                        auto end_token = next(it, output_vector.size() - 1);
                        if (!(*next_token == "(" && *end_token == ")") || output_vector.size() == 3) {
                            throw Undefined_syntax();
                        }
                    }
                }
            } else if (*it == DELETE || *it == PRINT || *it == WHO || *it == QUIT || *it == RESET) {
                throw Undefined_syntax();
            }
        }
    } catch (...) { throw; }
}

void even_brackets(vector<string>& output_vector) {
    int round_bracket = 0;
    int curly_bracket = 0;
    int pipeline = 0;
    int greater_than_sign = 0;
    try {
        for (auto& it : output_vector) {
            if (it == "(") {
                if (curly_bracket == 0) {
                    round_bracket++;
                } else {
                    throw Undefined_syntax();
                }
            }
            if (it == ")") {
                if (curly_bracket == 0 && round_bracket > 0) {
                    round_bracket--;
                } else {
                    throw Undefined_syntax();
                }
            }
            if (it == "{") {
                if (curly_bracket != 0) {
                    throw Undefined_syntax();
                } else {
                    curly_bracket++;
                }
            }
            if (it == "|") {
                if (curly_bracket != 1 || pipeline != 0) {
                    throw Undefined_syntax();
                } else {
                    pipeline++;
                }
            }
            if (it == "<") {
                if (curly_bracket != 1 || greater_than_sign != 0) {
                    throw Undefined_syntax();
                } else {
                    greater_than_sign++;
                }
            }
            if (it == ">") {
                if (curly_bracket != 1 || greater_than_sign != 1) {
                    throw Undefined_syntax();
                } else {
                    greater_than_sign--;
                }
            }
            if (it == "}") {
                if (curly_bracket != 1) {
                    throw Undefined_syntax();
                } else {
                    curly_bracket--;
                    if (pipeline != 0) {
                        pipeline--;
                    }
                }
            }
        }
        if (curly_bracket != 0 || pipeline != 0 || round_bracket != 0 || greater_than_sign != 0) {
            throw Undefined_syntax();
        }
    } catch (...) { throw; }
}

void graph_syntax_checker(vector<string>& output_vector) {
    bool pipline = false;
    bool in_brcket = false;
    try {
        for (auto it = output_vector.begin(); it != output_vector.end(); it++) {
            auto next_token = next(it, 1);
            if (*it == "{") {
                if (!(*next_token == "}")) {
                    in_brcket = true;
                } else {
                    it++;
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
                            throw Undefined_syntax();
                        }
                    } else {
                        throw Undefined_syntax();
                    }
                } else if (in_brcket && pipline) {
                    string first;
                    if (*it == "<") {
                        it++;
                        if (is_vertex(*it)) {
                            first = *it;
                            it++;
                            if (*it == ",") {
                                it++;
                                if (is_vertex(*it)) {
                                    if (*it == first) {
                                        throw Undefined_variable();
                                    }
                                    it++;
                                    if (*it == ">") {
                                        it++;
                                        if (*it == "}") {
                                            in_brcket = false;
                                        } else if (*it == ",") {
                                        } else {
                                            throw Undefined_syntax();
                                        }
                                    } else {
                                        throw Undefined_syntax();
                                    }
                                } else {
                                    throw Undefined_syntax();
                                }
                            } else {
                                throw Undefined_syntax();
                            }
                        } else {
                            throw Undefined_syntax();
                        }
                    } else {
                        throw Undefined_syntax();
                    }
                }
            }

        }
    } catch (...) { throw; }
}

void syntax_checker(vector<string>& output_vector) {
    try {
        equal_symbol_checker(output_vector);
        saved_function_checker(output_vector);
        even_brackets(output_vector);
        graph_syntax_checker(output_vector);
    } catch (...) { throw; }
}

bool is_over(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it) {
    auto next_token = next(it);
    if (next_token - token_vector.end() == 0) {
        return true;
    } else if ((*next_token)->name == ")") {
        auto next_token_2 = next(next_token, 1);
        if (next_token_2 - token_vector.end() == 0) {
            return true;
        } else {
            it++;//todo:: need to check
            return true;
        }
    } else return false;
}

shared_ptr<Graph> regex_evaluate(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                                 shared_ptr<Graph> first_arg = nullptr) {
    return evaluate(token_vector, it, first_arg);
}

void argument_finder(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                     shared_ptr<Graph>& arg) {
    try {
        if (is_graph((*it)->name)) {
            arg = (*it)->ptr;
        } else if ((*it)->name == "(") {
            it++;
            arg = regex_evaluate(token_vector, it);
        } else if ((*it)->name == "!" && it != token_vector.end()) {
            auto next_token = next(it, 1);
            if (is_graph((*next_token)->name)) {
                shared_ptr<Graph> temp(new Graph(!(*(*next_token)->ptr)));
                arg = temp;
                it++;
            } else if ((*next_token)->name == "(") {
                arg = regex_evaluate(token_vector, it);
            } else {
                throw Undefined_syntax();
            }
        } else {
            throw Undefined_syntax();
        }
    } catch (...) { throw; }
}

shared_ptr<Graph> math(shared_ptr<Graph>& first_arg, string& operator_symbol, shared_ptr<Graph>& second_arg) {
    try {
        shared_ptr<Graph> res_2;
        if (operator_symbol == "+") {
            shared_ptr<Graph> res(new Graph((*first_arg) + (*second_arg)));
            return res;
        }
        if (operator_symbol == "^") {
            shared_ptr<Graph> res(new Graph((*first_arg) ^ (*second_arg)));
            return res;
        }
        if (operator_symbol == "-") {
            shared_ptr<Graph> res(new Graph((*first_arg) - (*second_arg)));
            return res;
        }
        if (operator_symbol == "*") {
            shared_ptr<Graph> res(new Graph((*first_arg) * (*second_arg)));
            return res;
        }
    } catch (...) { throw; }
    assert("cant reach here");
    return shared_ptr<Graph>();
}

shared_ptr<Graph> evaluate(vector<shared_ptr<Token>>& token_vector, vector<shared_ptr<Token>>::iterator& it,
                           shared_ptr<Graph> first_arg) {
    string operator_symbol;
    shared_ptr<Graph> second_arg;
    try {
        if (first_arg == nullptr) {
            argument_finder(token_vector, it, first_arg);
            if (is_over(token_vector, it)) {
                return first_arg;
            }
            it++;
            if (is_operator((*it)->name)) {
                operator_symbol = (*it)->name;
                it++;
            } else {
                throw Undefined_syntax();
            }
            argument_finder(token_vector, it, second_arg);
            if (is_over(token_vector, it)) {
                return math(first_arg, operator_symbol, second_arg);
            } else {
                return evaluate(token_vector, it, math(first_arg, operator_symbol, second_arg));
            }
        } else {
            it++;
            if (is_operator((*it)->name)) {
                if (it == (token_vector.end()-1)){
                    throw Undefined_syntax();
                }
                operator_symbol = (*it)->name;
                it++;
            } else {
                throw Undefined_syntax();
            }

            argument_finder(token_vector, it, second_arg);
            if (is_over(token_vector, it)) {
                return math(first_arg, operator_symbol, second_arg);
            } else {
                return evaluate(token_vector, it, math(first_arg, operator_symbol, second_arg));
            }
        }
    } catch (...) { throw; }
}

void read(vector<shared_ptr<Token>>& token_vector, map<string, shared_ptr<Graph>>& graph_map, ostream& outfile) {
    try {
        if (!token_vector.empty()) {
            if ((*token_vector[0]).type == FUNCTION) {
                if ((*token_vector[0]).name == WHO) {
                    for (const auto& graph :graph_map) {
                        outfile << graph.first << endl;
                    }
                } else if ((*token_vector[0]).name == QUIT) {
                    exit(0);
                } else if ((*token_vector[0]).name == RESET) {
                    graph_map.clear();
                } else if ((*token_vector[0]).name == DELETE) {
                    graph_map.erase((*token_vector[2]).name);
                } else if ((*token_vector[0]).name == PRINT) {
                    Graph res;
                    auto it = token_vector.begin();
                    advance(it, 1);
                    (*evaluate(token_vector, it)).print(outfile);
                }
            } else {
                string graph_name = (*token_vector[0]).name;
                auto it = token_vector.begin();
                advance(it, 2);
                shared_ptr<Graph> graph = evaluate(token_vector, it);
                graph_map[graph_name] = graph;
            }
        } else {
        }
    } catch (...) { throw; }
}

vector<shared_ptr<Token>> tokenizer(vector<string>& output_vector, map<string, shared_ptr<Graph>>& graph_map) {
    vector<shared_ptr<Token>> new_token_vector;
    try {
        for (auto token = output_vector.begin(); token != output_vector.end(); token++) {

            if (*token == DELETE || *token == PRINT || *token == WHO || *token == QUIT || *token == RESET) {
                shared_ptr<Token> ptr(new Token(FUNCTION, *token, nullptr));
                new_token_vector.push_back(ptr);

            } else if (*token == "(" || *token == ")" || *token == "+" || *token == "-" || *token == "*" ||
                       *token == "^" ||
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
                        throw Undefined_variable();
                    }
                    shared_ptr<Token> ptr(new Token(GRAPH, *token, it->second));
                    new_token_vector.push_back(ptr);
                }

            } else if (*token == "{") {
                token++;
                set<string> vertices;
                set<vector<string>> edges;
                while (*token != "|" && *token != "}") {
                    if (*token == ",") {
                        token++;
                    } else {
                        if (vertices.find(*token) != vertices.end()) {
                            throw Undefined_variable();
                        }
                        vertices.insert((*token));
                        token++;
                    }
                }
                if (*token == "|") {
                    while (*token != "}") {
                        vector<string> edge;
                        token++;
                        token++;
                        if (vertices.find(*token) == vertices.end()) {
                            throw Undefined_variable();
                        }
                        edge.push_back(*token);
                        token++;
                        token++;
                        if (vertices.find(*token) == vertices.end()) {
                            throw Undefined_variable();
                        }
                        edge.push_back(*token);
                        token++;
                        token++;
                        if (edges.find(edge) != edges.end()) {
                            throw Undefined_variable();
                        }
                        edges.insert(edge);
                    }
                }
                if (*token == "}") {
                    shared_ptr<Graph> graph_ptr(new Graph(vertices, edges));
                    shared_ptr<Token> token_ptr(new Token(GRAPH, "newgraph", graph_ptr));
                    new_token_vector.push_back(token_ptr);
                }

            } else {
                assert("wrong syntax");
            }
        }
    } catch (...) { throw; }
    return new_token_vector;
}

int main(int argc, char** argv) {
    map<string, shared_ptr<Graph>> graph_map;
//    if (argc == 3) {
//        fstream newfile;
//        ofstream outfile;
//        outfile.open(argv[2], ios::out);
//        newfile.open(argv[1], ios::in); //open a file to perform read operation using file object
//        if (newfile.is_open() && outfile.is_open()) { //checking whether the file is open
//            string str;
//            while (getline(newfile, str)) { //read data from file object and put it into string.
//                try {
//                    vector<string> output_vector = parse_to_vector(str);
//                    syntax_checker(output_vector);
//                    vector<shared_ptr<Token>> token_vector = tokenizer(output_vector, graph_map);
//                    read(token_vector, graph_map, outfile);
//                }
//                catch (const bad_alloc& e) {
//                    cerr << e.what() << endl;
//                    newfile.close();
//                    graph_map.clear();
//                    exit(0);
//                } catch (const Undefined_variable& e) {
//                    outfile << e.what() << endl;
//                } catch (const Undefined_syntax& e) {
//                    outfile << e.what() << endl;
//                } catch (...) {
//                    cerr << "Error: something unexpected" << endl;
//                    newfile.close();
//                    graph_map.clear();
//                    exit(0);
//                }
//            }
//            newfile.close(); //close the file object.
//        }
//    } else if (argc == 1) {
//        string str;
//        while (true) {
//            try {
//                if(str == ""){
//                    continue;
//                }
//                cout << "Gcalc> ";
//                getline(cin, str);
//                vector<string> output_vector = parse_to_vector(str);
//                syntax_checker(output_vector);
//                vector<shared_ptr<Token>> token_vector = tokenizer(output_vector, graph_map);
//                read(token_vector, graph_map, cout);
//            } catch (const bad_alloc& e) {
//                cerr << e.what() << endl;
//                graph_map.clear();
//                exit(0);
//            } catch (const Undefined_variable& e) {
//                cout << e.what() << endl;
//            } catch (const Undefined_syntax& e) {
//                cout << e.what() << endl;
//            } catch (...) {
//                cerr << "Error: something unexpected" << endl;
//                graph_map.clear();
//                exit(0);
//            }
//        }
//    } else {
//        cerr << "Error: Too many arguments";
//        graph_map.clear();
//        exit(0);
//    }


    ifstream newfile;
    newfile.open("/Users/alexbondar/Technion/2_semester/matam/final_project/input_test.txt");
    ofstream outfile;
    outfile.open("/Users/alexbondar/Technion/2_semester/matam/final_project/outfile.txt");
    if (newfile.is_open() && outfile.is_open()) { //checking whether the file is open
        string str;
        while (getline(newfile, str)) { //read data from file object and put it into string.
            try {
                if (str == "print ((g1))") {
                }
                if (str.empty()) {
                    continue;
                }
                vector<string> output_vector = parse_to_vector(str);
                syntax_checker(output_vector);
                vector<shared_ptr<Token>> token_vector = tokenizer(output_vector, graph_map);
                read(token_vector, graph_map, outfile);
            }
            catch (const bad_alloc& e) {
                cerr << e.what() << endl;
                newfile.close();
                graph_map.clear();
                exit(0);
            } catch (const Undefined_variable& e) {
                outfile << e.what() << endl;
            } catch (const Undefined_syntax& e) {
                outfile << e.what() << endl;
            } catch (...) {
                cerr << "Error: something unexpected" << endl;
                newfile.close();
                graph_map.clear();
                exit(0);
            }
        }
        newfile.close(); //close the file object.
    }
    return 0;
}
