// Code for all tokens
#pragma once
#include <string>

enum class TokenType {
    // KEYWORD
    LET,
    IDENTIFIER,
    NUMBER,
    PRINT,

    // Math Operation
    PLUS,       // +
    MINUS,      // -
    STAR,       // *
    SLASH,      // /

    // Single character
    EQUAL,
    SEMICOLON,
    LPAREN,
    RPAREN,
    COMMENTS, // ##

    // Misc
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;
    int line;
    Token(TokenType type, const std::string& value, int line = 0)
        : type(type), value(value), line(line) {}
};