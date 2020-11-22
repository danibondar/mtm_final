cmake_minimum_required(VERSION 3.15)
project(final_project)

# set(CMAKE_CXX_STANDARD 11)
set(CMAKE_TEST_DEBUG "-std=c++11 -pedantic-errors -Wall -Werror")
set(CMAKE_TEST_RELEASE "${CMAKE_TEST_DEBUG} -DNDEBUG")

set(CMAKE_CXX_FLAGS ${CMAKE_TEST_DEBUG})

add_executable(final_project main.cpp graph.cpp python_interface.cpp gcalc.cpp gcalc.h utilities.cpp utilities.h evaluator.cpp evaluator.h commends.cpp commends.h input_stream.cpp input_stream.h parse_string.cpp parse_string.h tokenizer.cpp tokenizer.h reader.cpp reader.h)