#pragma once
#include <vector>
#include <memory>
#include <stdexcept>
#include "../lexer/token.hpp"
#include "../AST/expr.hpp"
#include "../AST/stmt.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& tokens)
        : tokens(tokens), current(0) {}

    std::vector<std::unique_ptr<Stmt>> parse() {
        std::vector<std::unique_ptr<Stmt>> statements;
        while (!isAtEnd()) {
            statements.push_back(parseStatement());
        }
        return statements;
    }

private:
    const std::vector<Token>& tokens;
    size_t current;

    const Token& peek() const { return tokens[current]; }
    const Token& previous() const { return tokens[current - 1]; }
    bool isAtEnd() const { return peek().type == TokenType::END_OF_FILE; }

    const Token& advance() {
        if (!isAtEnd()) current++;
        return previous();
    }

    bool check(TokenType type) const {
        return !isAtEnd() && peek().type == type;
    }

    bool match(TokenType type) {
        if (check(type)) {
            advance();
            return true;
        }
        return false;
    }

    void consume(TokenType type, const std::string& message) {
        if (check(type)) {
            advance();
        } else {
            throw std::runtime_error("Parse error: " + message);
        }
    }

    std::unique_ptr<Stmt> parseStatement() {
        if (match(TokenType::LET)) return parseLetStatement();
        if (match(TokenType::PRINT)) return parsePrintStatement();
        throw std::runtime_error("Unknown statement.");
    }

    std::unique_ptr<Stmt> parseLetStatement() {
        Token name = advance();
        consume(TokenType::EQUAL, "Expected '=' after identifier.");
        auto value = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after expression.");
        return std::make_unique<LetStmt>(name.value, std::move(value));
    }

    std::unique_ptr<Stmt> parsePrintStatement() {
        auto value = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after print.");
        return std::make_unique<PrintStmt>(std::move(value));
    }

    std::unique_ptr<Expr> parseExpression() {
        auto left = parsePrimary();

        while (match(TokenType::PLUS) || match(TokenType::MINUS) ||
               match(TokenType::STAR) || match(TokenType::SLASH)) {
            std::string op = previous().value;
            auto right = parsePrimary();
            left = std::make_unique<BinaryExpr>(std::move(left), op, std::move(right));
        }

        return left;
    }

    std::unique_ptr<Expr> parsePrimary() {
        if (match(TokenType::NUMBER)) {
            return std::make_unique<NumberExpr>(previous().value);
        }
        if (match(TokenType::IDENTIFIER)) {
            return std::make_unique<IdentifierExpr>(previous().value);
        }

        throw std::runtime_error("Expected expression.");
    }
};
