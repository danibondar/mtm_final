#include "input_stream.h"
#include "parse_string.h"
#include <vector>
input_stream::input_stream(int argc,  char** argv) {
    if (argc == 3) {
        is_manual = false;
        input_file.open(argv[1], ios::in);
        output_file.open(argv[2], ios::out);
        if (input_file.is_open() && output_file.is_open()) {
            //todo::throw something
        }
    } else if (argc == 1) {
        is_manual = true;
    } else {
        //todo::throw something
    }
}

vector<string> input_stream::get_line() {
    if (is_manual) {
        if (!cin.eof()) {
            cout << "Gcalc> ";
            if (getline(cin, input_str).eof()) {
                //todo::throw end of file - need to catch it at the while loop and do something
            }
        }
    } else {
        if (getline(input_file, input_str).eof()) {
            //todo::throw end of file - need to catch it at the while loop and do something
        }
    }
    return parse_string::parse_to_vector(input_str);
}

ostream& input_stream::get_outstream() {
    if (is_manual) {
        return cout;
    }
    return output_file;
}

void input_stream::close_file() {
    if(!is_manual){
        input_file.close();
        output_file.close();
    }
}

