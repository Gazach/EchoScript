#pragma once
#include <vector>
#include <cctype>
#include "../token.hpp"
#include "helper.hpp"

inline std::vector<Token> tokenize(const std::string& input) {
    scanner cursor(input);
    std::vector<Token> tokens;

    while (cursor.currentChar() != '\0') {
        cursor.skipWhitespace();
        char ch = cursor.currentChar();

        if (std::isalpha(ch)) {
            std::string ident = cursor.readWhile([](char c) { return std::isalnum(c) || c == '_'; });
            if (ident == "let") tokens.emplace_back(TokenType::LET);
            else if (ident == "print") tokens.emplace_back(TokenType::PRINT);
            else tokens.emplace_back(TokenType::IDENTIFIER, ident);
        }
        else if (std::isdigit(ch)) {
            std::string num = cursor.readWhile([](char c) {
                return std::isdigit(static_cast<unsigned char>(c));
            });
            tokens.emplace_back(TokenType::NUMBER, num);
        }

        else {
            switch (ch) {
                case '=': tokens.emplace_back(TokenType::EQUAL); break;
                case '+': tokens.emplace_back(TokenType::PLUS); break;
                case '-': tokens.emplace_back(TokenType::MINUS); break;
                case '*': tokens.emplace_back(TokenType::STAR); break;
                case '/': tokens.emplace_back(TokenType::SLASH); break;
                case '(': tokens.emplace_back(TokenType::LPAREN); break;
                case ')': tokens.emplace_back(TokenType::RPAREN); break;
                case ';': tokens.emplace_back(TokenType::SEMICOLON); break;
                default: tokens.emplace_back(TokenType::UNKNOWN, std::string(1, ch)); break;
            }
            cursor.advance();
        }
    }

    tokens.emplace_back(TokenType::END_OF_FILE);
    return tokens;
}
