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
#include "cheack_functions.h"


#define DELETE "delete"
#define WHO "who"
#define RESET "reset"
#define QUIT "quit"
#define PRINT "print"
#define SAVE "save"
#define LOAD "load"
#define FUNCTION "function"
#define REGEX "regex"
#define GRAPH "graph"
#define FILENAME "filename"
//using namespace std;

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
    //return !(str == DELETE || str == PRINT || str == WHO || str == QUIT || str == RESET);
    return true;
}

bool is_graph(const string& str) {
    if (str == DELETE || str == PRINT || str == WHO || str == QUIT || str == RESET || str == LOAD || str == SAVE) {
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

bool is_file(const string& str) {
    if (str.empty()) {
        return false;
    }
    for (auto& it :str) {
        if (it == '"' || it == ',' || it == ')' || it == '(' || it == '{' || it == '}' || it == '[' || it == ']') {
            return false;
        }
    }
    return true;
}
