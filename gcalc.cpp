#include "gcalc.h"
#include "Exceptions.h"
#include "tokenizer.h"
#include "reader.h"

void gcalc::activate_gcalc() {
    while (true) {
        try {
            vector<string> input_string = stream.get_line();
            tokenizer token_vector(graph_map);
            reader c_read(&graph_map, stream.get_outstream());
            c_read.read(token_vector.to_token(input_string));
        } catch (const Quit& e) {
            stream.close_file();
            exit(0);
        }
        catch (const Exception& e) {
            stream.get_outstream() << e.what();
        } catch (...) {
            stream.close_file();
            exit(0);
        }
    }
}

gcalc::gcalc(int argc, char**& argv) :
        stream(argc, argv) {}
