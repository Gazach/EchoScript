#include "parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

const Token& Parser::peek() {
    return tokens[current];
}

const Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

const Token& Parser::previous() {
    return tokens[current - 1];
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}

void Parser::consume(TokenType type, const std::string& message) {
    if (peek().type != type)
        throw std::runtime_error("Parser error: " + message);
    advance();
}

StmtPtr Parser::statement() {
    if (match(TokenType::LET)) return letStatement();
    if (match(TokenType::PRINT)) return printStatement();
    throw std::runtime_error("Unknown statement");
}

StmtPtr Parser::letStatement() {
    Token name = peek(); consume(TokenType::IDENTIFIER, "Expected variable name.");
    consume(TokenType::EQUAL, "Expected '='.");
    ExprPtr value = expression();
    consume(TokenType::SEMICOLON, "Expected ';'.");
    return std::make_shared<LetStmt>(name.lexeme, value);
}

StmtPtr Parser::printStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '('.");
    ExprPtr value = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')'.");
    consume(TokenType::SEMICOLON, "Expected ';'.");
    return std::make_shared<PrintStmt>(value);
}

ExprPtr Parser::expression() {
    Token token = peek();
    if (match(TokenType::NUMBER)) {
        return std::make_shared<LiteralExpr>(std::stoi(token.lexeme));
    }
    else if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<VariableExpr>(token.lexeme);
    }
    throw std::runtime_error("Invalid expression.");
}

std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> stmts;
    while (!isAtEnd()) {
        stmts.push_back(statement());
    }
    return stmts;
}
