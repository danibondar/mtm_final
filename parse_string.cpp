#include "parse_string.h"
#include <vector>
vector<string> parse_string::parse_to_vector(const string& str) {
    vector<string> string_vector;
    string temp_str;
    try {
        for (char it :str) {
            if (it == ' ') {
                if (!temp_str.empty()) {
                    string_vector.push_back(temp_str);
                    temp_str = "";
                }
                continue;
            }
            if (utilities::is_alpha_numerical_char(it) || it == ']' || it == '[' || it == ';') {
                temp_str += it;
            } else {
                if (!temp_str.empty()) {
                    string_vector.push_back(temp_str);
                    temp_str = "";
                }
                temp_str += it;//TODO::find a better wat to convert char to string
                string_vector.push_back(temp_str);
                temp_str = "";
            }
        }
        if (!temp_str.empty()) {
            string_vector.push_back(temp_str);
        }
    } catch (...) { throw; }
    return string_vector;
}