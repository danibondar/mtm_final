#ifndef FINAL_PROJECT_EXCEPTIONS_H
#define FINAL_PROJECT_EXCEPTIONS_H

#include <iostream>
#include <fstream>
#define DEFAULT_ERROR_PREFIX "Error: "

class Exception : public std::exception {
};

class Undefined_variable : public Exception {
public:
    const char* what() const noexcept override {
        return DEFAULT_ERROR_PREFIX "Undefined variable\n";
    }
};

class Quit : public Exception {
public:
};

class Undefined_syntax : public Exception {
public:
    const char* what() const noexcept override {
        return DEFAULT_ERROR_PREFIX "Undefined syntax\n";
    }
};

#endif //FINAL_PROJECT_EXCEPTIONS_H
