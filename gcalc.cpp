#include <iostream>
#include <memory>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <stdbool.h>

#define DELETE "delete"
#define WHO "who"
#define RESET "reset"
#define QUIT "quit"
#define PRINT "print"
using namespace std;
G& evaluate(vector<string>::iterator& it, G first_arg = nullptr);

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
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        if (is_symbol(*it)) {
            return false;
        }
        if (*it == '[') {
            square_bracket++;
        }
        if (*it == ']') {
            if (square_bracket <= 0) {
                return false;
            } else { square_bracket--; }
        }
        if (*it == ';') {
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
        for (string::const_iterator it = str.begin(); it != str.end(); it++) {
            if (*it < '0' || (*it > '9' && *it < 'A') || (*it > 'Z' && *it < 'a') || *it > 'z') {
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

vector<string> parse_to_vector(string str) {
    vector<string> input_stack;
    string temp_str;
    for (string::iterator it = str.begin(); it != str.end(); it++) {
        if (*it == ' ' and !temp_str.empty()) {
            input_stack.push_back(temp_str);
            temp_str = "";
        }
        if (is_symbol(*it)) {
            if (!temp_str.empty()) {
                input_stack.push_back(temp_str);
                temp_str = *it;
                input_stack.push_back(temp_str);
                temp_str = "";
            } else {
                temp_str = *it;
                input_stack.push_back(temp_str);
                temp_str = "";
            }
        } else {
            //not a symbol
            temp_str.push_back(*it);
        }
    }
    if (!temp_str.empty()) {
        input_stack.push_back(temp_str);
    }
    return input_stack;
}

void saved_function_checker(vector<string>& output_vector) {
    bool saved_function = false;
    for (vector<string>::iterator it = output_vector.begin(); it != output_vector.end(); it++) {
        if (it == output_vector.begin()) {
            if (*it == DELETE || *it == PRINT || *it == WHO || *it == QUIT || *it == RESET) {
                saved_function = true;
                if (*it == WHO || *it == QUIT || *it == RESET) {
                    if (output_vector.size() != 1) {
                        //todo:: exception wrong syntax
                    }
                } else {
                    auto next_token = next(it, 1);
                    if (!(*next_token == "(" && *output_vector.end() == ")")) {
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
    for (vector<string>::iterator it = output_vector.begin(); it != output_vector.end(); it++) {
        if (*it == "=") {
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
    for (vector<string>::iterator it = output_vector.begin(); it != output_vector.end(); it++) {
        if (*it == "(") {
            if (curly_bracket == 0) {
                round_bracket++;
            } else {
                //todo:: exception wrong syntax
            }
        }
        if (*it == ")") {
            if (curly_bracket == 0 && round_bracket > 0) {
                round_bracket--;
            } else {
                //todo:: exception wrong syntax
            }
        }
        if (*it == "{") {
            if (curly_bracket != 0) {
                //todo:: exception wrong syntax
            } else {
                curly_bracket++;
            }
        }
        if (*it == "|") {
            if (curly_bracket != 1 || pipeline != 0) {
                //todo:: exception wrong syntax
            } else {
                pipeline++;
            }
        }
        if (*it == "<") {
            if (curly_bracket != 1 || greater_than_sign != 0) {
                //todo:: exception wrong syntax
            } else {
                greater_than_sign++;
            }
        }
        if (*it == ">") {
            if (curly_bracket != 1 || greater_than_sign != 1) {
                //todo:: exception wrong syntax
            } else {
                greater_than_sign--;
            }
        }
        if (*it == "}") {
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

    for (vector<string>::iterator it = output_vector.begin(); it != output_vector.end(); it++) {
        auto next_token = next(it, 1);
        if (*it == "{") {
            if (!(*next_token == "}")) {
                in_brcket = true;
            } else {
                it += 2;
            }
        }else {
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
                    if (is_vertex(*it)){
                        it++;
                        if (*it==","){
                            it++;
                            if (is_vertex(*it)){
                                it++;
                                if (*it == ">"){
                                    it++;
                                    if (*it == "}"){
                                        it++;
                                        in_brcket = false;
                                    } else if (*it == ","){
                                        it++;
                                    } else{
                                        //todo:: exception wrong syntax
                                    }
                                } else{
                                    //todo:: exception wrong syntax
                                }
                            } else{
                                //todo:: exception wrong syntax
                            }
                        } else{
                            //todo:: exception wrong syntax
                        }
                    } else{
                        //todo:: exception wrong syntax
                    }
                } else{
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

/*
void read(vector<string> output_vector) {
    if (!output_vector.empty()) {
        if (output_vector[0] == DELETE || output_vector[0] == PRINT || output_vector[0] == WHO
            || output_vector[0] == QUIT || output_vector[0] == RESET) {
            //TODO:: sum functions
        } else {
            //equal symbol
            if (output_vector[1] == "=") {
                if (is_graph(output_vector[0])){

                } else{
                    //todo:: exeption not a corect syntext for a graph
                }
                //TODO:: sum functions
            }
        }
    }
}
*/
bool is_over(vector<string> output_vector, vector<string>::iterator& it) {
    if (*it == ")") {
        if (it == output_vector.end()) {
            return true;
        } else {
            it++;
            return true;
        }
    } else if (it == output_vector.end()) {
        return true;
    } else {
        return false;
    }
}

G& regex_evaluate(vector<string> output_vector, vector<string>::iterator& it, G& first_arg = nullptr) {
    return evaluate(output_vector, it, first_arg);
}

void argument_finder(vector<string> output_vector, vector<string>::iterator& it, G& arg) {
    if (is_graph(*it)) {
        arg = string_to_graph(*it);
        it++;
    } else if (*it == "(") {
        it++;
        arg = regex_evaluate(output_vector, it);
    } else if (*it == "!") {
        auto next_token = next(it, 1);
        if (is_graph(*next_token)) {
            arg = !string_to_graph(*it);
        } else if (*next_token == "(") {
            arg = !regex_evaluate(output_vector, it);
        } else {
            //TODO::exception wrong syntax
        }
    } else {
        //TODO::exception wrong syntax
    }
}

G& evaluate(vector<string> output_vector, vector<string>::iterator& it, G& first_arg = nullptr) {
    string operator_symbol;
    G second_arg;
    if (first_arg == nullptr) {
        auto next_token = next(it, 1);
        if (*it == "(" && *next_token == ")") {
            it += 2;
            return first_arg;
        }
        next_token = next(it, 2);
        if (*it == "(" && *next_token == ")") {
            it++;
            G temp = string_to_graph(*it);
            it++;
            return temp;

        }
        argument_finder(output_vector, it, first_arg);
        if (is_operator(*it)) {
            operator_symbol = *it;
            it++;
        } else {
            //TODO:: exeption not correct syntext
        }
        argument_finder(output_vector, it, second_arg);
        if (is_over(output_vector, it)) {
            return math();
        } else {
            return evaluate(output_vector, it, math());
        }
    } else {
        if (is_operator(*it)) {
            operator_symbol = *it;
            it++;
        } else {
            //TODO:: exception not correct syntax
        }

        argument_finder(output_vector, it, second_arg);
        if (is_over(output_vector, it)) {
            return math();
        } else {
            return evaluate(output_vector, it, math());
        }
    }
}

int main() {

    while (true) {
        string str;
        cout << "Gcalc> ";
        getline(cin, str);
        vector<string> output_vector = parse_to_vector(str);
        try {
            syntax_checker(output_vector);
        }
        catch (//TODO::EXCEPTION)
                vector<string>::iterator it = output_vector.begin();
        G res = evaluate(output_vector, it);
    }
    return 0;
}
