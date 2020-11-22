
#include "commends.h"
#include "graph.h"
#include "evaluator.h"


void commends::who() {
    try {
        if (token_vector.size() != 1) {
            throw Undefined_syntax();
        }
        for (const auto& graph :graph_map) {
            outfile << graph.first << endl;
        }
    } catch (...) { throw; }
}

void commends::print() {
    try {
        if (token_vector.size() == 1) {
            throw Undefined_syntax();
        }
        Graph res;
        auto it = token_vector.cbegin() + 1;
        evaluator eval(token_vector, it);
        shared_ptr<Graph> res_eval = eval.evaluate();
        if (res_eval == nullptr) {
            throw Undefined_variable();
        }
        (*res_eval).print(outfile);
    } catch (...) { throw; }
}

void commends::write_to_file(const shared_ptr<Graph>& graph, const shared_ptr<Token>& file_name) {
    ofstream save_file((*file_name).get_name(), ios_base::binary);
    try {
        if (save_file.is_open()) {
            vector<string> vertices;
            unsigned int size_vertices = graph->get_connections().size();
            unsigned int size_edges = 0;
            for (auto& connection:graph->get_connections()) {
                vertices.push_back(connection.first);
                size_edges += connection.second.size();
            }
            save_file.write((const char*) &size_vertices, sizeof(size_vertices));
            save_file.write((const char*) &size_edges, sizeof(size_edges));
            for (auto& vertex:vertices) {
                unsigned int sz = vertex.size();
                save_file.write((const char*) &sz, sizeof(sz));
                save_file.write((const char*) vertex.c_str(), sz);
            }
            for (auto& connection:graph->get_connections()) {
                for (auto& dest :connection.second) {
                    unsigned int sz = connection.first.size();
                    save_file.write((const char*) &sz, sizeof(sz));
                    save_file.write((const char*) connection.first.c_str(), sz);

                    sz = dest.size();
                    save_file.write((const char*) &sz, sizeof(sz));
                    save_file.write((const char*) dest.c_str(), sz);
                }
            }
            save_file.close();
        } else {
            throw Undefined_variable();
        }
    } catch (const ostream::failure& e) {
        throw Undefined_variable();
    } catch (...) {
        if (save_file.is_open()) {
            save_file.close();
        }
        throw;
    }
}

void commends::save() {
    try {
        if (token_vector.size() != 1) {
            throw Undefined_syntax();
        }
        auto end_itr = token_vector.end() - 3;
        auto start_itr = token_vector.begin() + 2;
        vector<shared_ptr<Token> > new_token_vector(start_itr, end_itr);
        auto it = new_token_vector.cbegin();
        if (new_token_vector.empty()) {
            throw Undefined_syntax();
        }
        evaluator eval(new_token_vector, it);
        shared_ptr<Graph> graph = eval.evaluate();
        write_to_file(graph, *next(token_vector.end(), -2));//TODO:: save the filename as one token
    } catch (...) { throw; }
}

void commends::restart() {
    try {
        if (token_vector.size() != 1) {
            throw Undefined_syntax();
        }
        graph_map.clear();
    } catch (...) { throw; }
}

void commends::delete_graph() {
    try {
        if (token_vector.size() != 4 || graph_map.find((token_vector[2])->get_name()) == graph_map.end() ||
            (token_vector[1])->get_name() != "(" || (token_vector[3])->get_name() != ")") {
            throw Undefined_syntax();
        }
        graph_map.erase((token_vector[2])->get_name());
    } catch (...) { throw; }
}

void commends::quit() {
    try {
        if (token_vector.size() != 1) {
            throw Undefined_syntax();
        }
        graph_map.~map();
        throw Quit();
    } catch (...) { throw; }
}

commends::commends(const vector<shared_ptr<Token>>& token_vector, map<string, shared_ptr<Graph>>& graph_map,
        ostream& outfile) : token_vector(token_vector), graph_map(graph_map),outfile(outfile) {}
