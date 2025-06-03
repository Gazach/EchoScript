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
    PLUS,
    MINUS,
    STAR,
    SLASH,

    // Single character
    EQUAL,
    SEMICOLON,
    LPAREN,
    RPAREN,

    // 
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value = "")
        : type(type), value(value) {}
};