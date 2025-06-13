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
            if (ident == "let") tokens.emplace_back(TokenType::LET, ident, cursor.getLine());
            else if (ident == "print") tokens.emplace_back(TokenType::PRINT, ident, cursor.getLine());
            else tokens.emplace_back(TokenType::IDENTIFIER, ident, cursor.getLine());
        }
        else if (std::isdigit(ch)) {
            std::string num = cursor.readWhile([](char c) {
                return std::isdigit(static_cast<unsigned char>(c));
            });
            tokens.emplace_back(TokenType::NUMBER, num, cursor.getLine());
        }
        else {
            switch (ch) {
                case '+': tokens.emplace_back(TokenType::PLUS, "+", cursor.getLine()); break;
                case '-': tokens.emplace_back(TokenType::MINUS, "-", cursor.getLine()); break;
                case '*': tokens.emplace_back(TokenType::STAR, "*", cursor.getLine()); break;
                case '/': tokens.emplace_back(TokenType::SLASH, "/", cursor.getLine()); break;
                case '=': tokens.emplace_back(TokenType::EQUAL, "=", cursor.getLine()); break;
                case ';': tokens.emplace_back(TokenType::SEMICOLON, ";", cursor.getLine()); break;
                case '(': tokens.emplace_back(TokenType::LPAREN, "(", cursor.getLine()); break;
                case ')': tokens.emplace_back(TokenType::RPAREN, ")", cursor.getLine()); break;
                case '#':
                    cursor.advance(); // Consume first '#'
                    if (cursor.currentChar() == '#') {
                        while (cursor.currentChar() != '\n' && cursor.currentChar() != '\0') {
                            cursor.advance();
                        }
                    } else {
                        throw std::runtime_error("at Line " + std::to_string(cursor.getLine()) + ": Unexpected single '#' character. Did you mean '##' for a comment?");
                    }
                    continue; // Skip adding a token
                case '\0': tokens.emplace_back(TokenType::END_OF_FILE, "", cursor.getLine()); break;
                default:
                    tokens.emplace_back(TokenType::UNKNOWN, std::string(1, ch), cursor.getLine()); break;
            }
            cursor.advance();
        }

    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", cursor.getLine());

    return tokens;
}
