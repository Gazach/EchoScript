#include "lexer.hpp"
#include <cctype>
#include <unordered_map>
#include <stdexcept>

Lexer::Lexer(const std::string& src) : source(src) {}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

char Lexer::advance() {
    return source[current++];
}

char Lexer::peek() {
    return isAtEnd() ? '\0' : source[current];
}

char Lexer::peekNext() {
    return (current + 1 < source.length()) ? source[current + 1] : '\0';
}

bool Lexer::match(char expected) {
    if (isAtEnd() || source[current] != expected) return false;
    current++;
    return true;
}

void Lexer::addToken(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, line);
}

void Lexer::addToken(TokenType type, const std::string& text) {
    tokens.emplace_back(type, text, line);
}

TokenType Lexer::checkKeyword(const std::string& word) {
    static std::unordered_map<std::string, TokenType> keywords = {
        {"let", TokenType::LET},
        {"func", TokenType::FUNC},
        {"return", TokenType::RETURN},
        {"print", TokenType::PRINT},
        {"println", TokenType::PRINTLN},
        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
    };

    auto it = keywords.find(word);
    return it != keywords.end() ? it->second : TokenType::IDENTIFIER;
}

void Lexer::identifier() {
    while (isalnum(peek()) || peek() == '_') advance();
    std::string text = source.substr(start, current - start);
    addToken(checkKeyword(text));
}

void Lexer::number() {
    while (isdigit(peek())) advance();
    addToken(TokenType::NUMBER);
}

void Lexer::string() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line++;
        advance();
    }

    if (isAtEnd()) return;

    advance(); // closing quote
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

void Lexer::scanToken() {
    char c = advance();
    switch (c) {
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case ',': addToken(TokenType::COMMA); break;
    case '+': addToken(TokenType::PLUS); break;
    case '-': addToken(TokenType::MINUS); break;
    case '*': addToken(TokenType::STAR); break;
    case '/': addToken(TokenType::SLASH); break;
    case '=': addToken(TokenType::EQUAL); break;
    case '"': string(); break;
    case '#':
        if (match('#')) {
            // Skip the comment until the end of line
            while (peek() != '\n' && !isAtEnd()) advance();
        }
        else {
            addToken(TokenType::UNKNOWN); // Or handle as syntax error
        }
        break;
    case '\'':
        charLiteral();
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;
    default:
        if (isdigit(c)) {
            number();
        }
        else if (isalpha(c) || c == '_') {
            identifier();
        }
        else {
            addToken(TokenType::UNKNOWN);
        }
        break;
    }
}
void Lexer::scanString() {
    std::string value;
    advance();  // Skip the opening quote

    while (peek() != '"' && !isAtEnd()) {
        value += peek();
        advance();
    }

    if (isAtEnd()) {
        throw std::runtime_error("Unterminated string.");
    }

    advance();  // Skip the closing quote

    addToken(TokenType::STRING, value);
}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", line);
    return tokens;
}

// In lexer.cpp - Check charLiteral()
void Lexer::charLiteral() {
    advance(); // Skip opening '
    start = current;

    // Handle double '' escape
    if (peek() == '\'') {
        advance(); // Skip first '
        if (peek() != '\'') {
            throw std::runtime_error("Invalid empty character literal");
        }
        addToken(TokenType::CHAR, "'"); // Store single quote
        advance(); // Skip second '
    } else {
        char value = advance();
        addToken(TokenType::CHAR, std::string(1, value));
    }

    // Consume closing '
    if (advance() != '\'') {
        throw std::runtime_error("Unterminated character literal");
    }
}