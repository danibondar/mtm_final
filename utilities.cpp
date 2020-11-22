#include "utilities.h"
//TODO::find a way to compact all of the defines
#define DELETE "delete"
#define WHO "who"
#define RESET "reset"
#define QUIT "quit"
#define PRINT "print"
#define SAVE "save"
#define LOAD "load"
#define FUNCTION "function"


bool utilities::is_alpha_numerical_char(const char& it) {
    return (it >= 'a' && it <= 'z') || (it >= 'A' && it <= 'Z') || (it >= '0' && it <= '9');
}

bool utilities::is_saved_word(const string& str) {
    return str == DELETE || str == PRINT || str == WHO || str == QUIT || str == RESET || str == LOAD || str == SAVE;
}

bool utilities::is_punctuation(const string& str) {
    return str == "(" || str == ")" || str == "+" || str == "-" || str == "*" || str == "^" ||
           str == "!" || str == "=" || str == ".";
}

bool utilities::is_alpha_numerical_string(const string& str) {
    for (const auto& it :str) {
        if (!is_alpha_numerical_char(it)) { return false; }
    }
    return true;
}

bool utilities::is_operator(const string& str) {
    return str == "+" || str == "^" || str == "*" || str == "-";
}

bool utilities::is_graph(const string& str) {
    if (is_saved_word(str)) {
        return false;
    }
    if ((str[0] >= 'a' && str[0] <= 'z') || (str[0] >= 'A' && str[0] <= 'Z')) {
        return is_alpha_numerical_string(str);
    }
    return false;
}

bool utilities::is_vertex(const string& str) {
    int square_bracket = 0;
    for (char it : str) {
        if (!is_alpha_numerical_char(it) && it != ']' && it != '[' && it != ';') {
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
    return  (square_bracket == 0);
}