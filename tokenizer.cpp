#include "tokenizer.h"
#include "utilities.h"
#include "Exceptions.h"

#define FUNCTION "function"
#define OPERATOR "operator"
#define NEW_GRAPH "new graph"
#define IDENTIFIER "identifier"


shared_ptr<Graph> tokenizer::graph_builder(const vector<string>& vector_string) {
    bool pipeline = false;
    map<string, set<string>> temp_graph;
    for (auto it = vector_string.begin(); it != vector_string.end(); it++) {
        if (*it == "{" || *it == "}" || *it == ",") { continue; }
        else if (!pipeline) {
            if (utilities::is_vertex(*it)) {
                temp_graph[*it];
            } else if (*it == "|") {
                pipeline = true;
            } else {
                throw Undefined_syntax();
            }
        } else {
            string s_vertex;
            string d_vertex;
            bool got_to_the_end = false;
            //TODO::think of a cleaner way to check this:
            if (*it == "<") {
                it++;
                if (utilities::is_vertex(*it)) {
                    s_vertex = *it;
                    it++;
                    if (*it == ",") {
                        it++;
                        if (utilities::is_vertex(*it)) {
                            d_vertex = *it;
                            it++;
                            if (*it == ">") {
                                got_to_the_end = true;
                            }
                        }
                    }
                }
            }
            if (!got_to_the_end) {
                throw Undefined_syntax();
            }
            if (temp_graph.find(s_vertex) == temp_graph.end() || temp_graph.find(d_vertex) == temp_graph.end()) {
                throw Undefined_variable();
            } else {
                temp_graph[s_vertex].insert(d_vertex);
            }
        }
    }
    shared_ptr<Graph> ptr(new Graph(temp_graph));
    return ptr;
}

vector<shared_ptr<Token>> tokenizer::to_token(const vector<string>& vector_string) {
    vector<shared_ptr<Token> > token_vector;
    try {
        bool is_in_brackets = false;
        vector<string> graph_builder_vector;

        for (const auto& str_it : vector_string) {
            if (is_in_brackets) {
                graph_builder_vector.push_back(str_it);
                if (str_it == "}") {
                    is_in_brackets = false;
                    shared_ptr<Graph> ptr_new_graph = graph_builder(graph_builder_vector);
                    shared_ptr<Token> ptr(new Token(IDENTIFIER, NEW_GRAPH, ptr_new_graph));
                    token_vector.push_back(ptr);
                }
            } else if (str_it == "{") {
                is_in_brackets = true;
                graph_builder_vector.push_back(str_it);
            } else if (utilities::is_saved_word(str_it)) {
                shared_ptr<Token> ptr(new Token(FUNCTION, str_it, nullptr));
                token_vector.push_back(ptr);
            } else if (utilities::is_punctuation(str_it)) {
                shared_ptr<Token> ptr(new Token(OPERATOR, str_it, nullptr));
                token_vector.push_back(ptr);
            } else if (utilities::is_alpha_numerical_string(str_it)) {
                shared_ptr<Graph> graph_ptr = nullptr;
                auto temp_ptr = graph_map.find(str_it);
                if (temp_ptr != graph_map.end()) {
                    graph_ptr = temp_ptr->second;
                }
                shared_ptr<Token> ptr(new Token(IDENTIFIER, str_it, graph_ptr));
                token_vector.push_back(ptr);
            } else {
                throw Undefined_syntax();
            }
        }
        if (is_in_brackets) {
            throw Undefined_syntax();
        }
    } catch (...) { throw; }
    return token_vector;
}
