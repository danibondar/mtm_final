#ifndef FINAL_PROJECT_UTILITIES_H
#define FINAL_PROJECT_UTILITIES_H
#include <cstdlib>
#include <fstream>

using namespace std;
class utilities {
public:
    static bool is_alpha_numerical_char(const char& it);
    static bool is_alpha_numerical_string(const string& str);
    static bool is_saved_word(const string& str);
    static bool is_punctuation(const string& str);
    static bool is_graph(const string& str);
    static bool is_operator(const string& str);
    static bool is_vertex(const string& str);
};


#endif //FINAL_PROJECT_UTILITIES_H
