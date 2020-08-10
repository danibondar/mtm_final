CC = /usr/local/bin/gcc
#CC = /usr/bin/gcc
CXX=/usr/local/bin/g++
#CXX=/usr/bin/g++
COMP_FLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -c -fPIC
LINK_FLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -g
OBJS = $(GRAPH) $(TOKEN) $(MAIN) $(CHECK_FUNC)
GRAPH = graph.o
TOKEN = tokens.o
MAIN = gcalc.o
PYTHON_INTER = python_interface.o
CHECK_FUNC = checkfunc.o
GCALC = gcalc
PDF = design.pdf
MAKEFILE = Makefile
TESTS = test_in.txt test_out.txt
IFILE = graph.i
ZIPFILE = $(PDF) $(MAKEFILE) $(TESTS) cheack_functions.cpp cheack_functions.h Exceptions.h gcalc.cpp gcalc.h graph.cpp\
graph.h graph.i python_interface.cpp python_interface.h tokens.cpp tokens.h _graph.so
PYOBJS = $(GRAPH)

$(GCALC) : $(OBJS)
	$(CXX) $(LINK_FLAGS) $(OBJS) -o $@
$(MAIN) : gcalc.cpp gcalc.h Exceptions.h $(GRAPH) $(TOKEN) $(CHECK_FUNC)
	$(CXX) $(COMP_FLAGS) gcalc.cpp -o $@
$(GRAPH) : graph.cpp graph.h
	$(CXX) $(COMP_FLAGS) graph.cpp -o $@
$(TOKEN) : tokens.cpp tokens.h
	$(CXX) $(COMP_FLAGS) tokens.cpp -o $@
$(CHECK_FUNC) : cheack_functions.cpp cheack_functions.h
	$(CXX) $(COMP_FLAGS) cheack_functions.cpp -o $@
$(PYTHON_INTER) : python_interface.cpp python_interface.h $(GRAPH) $(CHECK_FUNC)
	$(CXX) $(COMP_FLAGS) python_interface.cpp -o $@
clean :
	rm -f $(OBJS) $(GCALC)
tar :
	zip $(GCALC).zip $(ZIPFILE)
libgraph.a: $(PYTHON_INTER) $(GRAPH) $(CHECK_FUNC)
	ar -rs $@ $^
	#swig -c++ -python graph.i
	#g++ -DNDEBUG -std=c++11 --pedantic-errors -Wall -I/usr/local/include/python3.6m -fPIC -shared graph_wrap.cxx python_interface.cpp libgraph.a -o _graph.so