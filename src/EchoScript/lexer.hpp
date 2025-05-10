#pragma once
#include "token.hpp"
#include <vector>

class Lexer {
public:
    Lexer(const std::string& source);
    std::vector<Token> tokenize();

private:
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    char peek();
    char peekNext();
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& text);
    void scanToken();
    void identifier();
    void number();
    void string();
    bool match(char expected);
    TokenType checkKeyword(const std::string& word);

};