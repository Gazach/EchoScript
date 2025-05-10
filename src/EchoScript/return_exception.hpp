// return_exception.hpp
#pragma once
#include "value.hpp"  // Assuming Value class is defined here

class ReturnException : public std::exception {
public:
    Value value;

    explicit ReturnException(const Value& v) : value(v) {}

    const char* what() const noexcept override {
        return "Function return exception";
    }
};
