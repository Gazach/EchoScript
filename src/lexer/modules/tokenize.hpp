#pragma once
#include <vector>
#include <cctype>
#include <stdexcept>
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
            if (ident == "let") tokens.emplace_back(TokenType::LET, ident);
            else if (ident == "print") tokens.emplace_back(TokenType::PRINT, ident);
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
                case '+': tokens.emplace_back(TokenType::PLUS, "+"); cursor.advance(); break;
                case '-': tokens.emplace_back(TokenType::MINUS, "-"); cursor.advance(); break;
                case '*': tokens.emplace_back(TokenType::STAR, "*"); cursor.advance(); break;
                case '/': tokens.emplace_back(TokenType::SLASH, "/"); cursor.advance(); break;
                case '=': tokens.emplace_back(TokenType::EQUAL, "="); cursor.advance(); break;
                case ';': tokens.emplace_back(TokenType::SEMICOLON, ";"); cursor.advance(); break;
                case '(': tokens.emplace_back(TokenType::LPAREN, "("); cursor.advance(); break;
                case ')': tokens.emplace_back(TokenType::RPAREN, ")"); cursor.advance(); break;
                case '#':
                    cursor.advance(); // Consume first '#'
                    if (cursor.currentChar() == '#') {
                        // It's a comment, skip until end of line
                        while (cursor.currentChar() != '\n' && cursor.currentChar() != '\0') {
                            cursor.advance();
                        }
                    } else {
                        // Single '#' is invalid → syntax error
                       throw std::runtime_error("Line " + std::to_string(cursor.getLine()) + ": Unexpected single '#' character. Did you mean '##' for a comment?");

                    }
                    break;
                case '\0': tokens.emplace_back(TokenType::END_OF_FILE, ""); break;
                default: tokens.emplace_back(TokenType::UNKNOWN, std::string(1, ch)); cursor.advance(); break;
            }
            cursor.advance();
        }
    }

    tokens.emplace_back(TokenType::END_OF_FILE);
    return tokens;
}
