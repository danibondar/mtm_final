#ifndef FINAL_PROJECT_INPUT_STREAM_H
#define FINAL_PROJECT_INPUT_STREAM_H

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

class input_stream {
private:
    bool is_manual;
    string input_str;
    ifstream input_file;
    ofstream output_file;
public:
    input_stream(int argc, char** argv);
    vector<string> get_line();
    ostream& get_outstream();
    void close_file();
};

#endif //FINAL_PROJECT_INPUT_STREAM_H