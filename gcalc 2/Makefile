CC = /usr/local/bin/gcc
#CXX=/usr/local/bin/g++
CXX=/usr/bin/g++
COMP_FLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -c
LINK_FLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG -g
OBJS = $(GRAPH) $(TOKEN) $(MAIN)
GRAPH = graph.o
TOKEN = tokens.o
MAIN = gcalc.o
GCALC = gcalc
PDF = design.pdf
MAKEFILE = Makefile
TESTS = test_in.txt test_out.txt
IFILE = graph.i
ZIPFILE = $(PDF) $(MAKEFILE) $(TESTS)
PYOBJS = $(GRAPH)

$(GCALC) : $(OBJS)
	$(CXX) $(LINK_FLAGS) $(OBJS) -o $@
$(MAIN) : gcalc.cpp gcalc.h Exceptions.h $(GRAPH) $(TOKEN)
	$(CXX) $(COMP_FLAGS) gcalc.cpp -o $@
$(GRAPH) : graph.cpp graph.h
	$(CXX) $(COMP_FLAGS) graph.cpp -o $@
$(TOKEN) : tokens.cpp tokens.h
	$(CXX) $(COMP_FLAGS) tokens.cpp -o $@
clean :
	rm -f $(OBJS) $(GCALC)
tar :
	zip $(GCALC).zip $(ZIPFILE)
lidgraph.a: $(GCALC) python_interface.h
	ar -rs $@ $^