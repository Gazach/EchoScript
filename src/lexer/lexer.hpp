/*This code work as umbrella*/
#pragma once
#include "modules/tokenize.hpp"

class Lexer {
public:
    explicit Lexer(const std::string& input) : input(input) {}

    std::vector<Token> tokenize() {
        return ::tokenize(input);
    }

private:
    std::string input;
};
