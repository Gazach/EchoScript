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

const Token& Parser::consume(TokenType type, const std::string& message) {
    if (peek().type != type)
        throw std::runtime_error("Parser error: " + message);
    return advance();
}

StmtPtr Parser::statement() {
    if (match(TokenType::LET)) return letStatement();
    if (match(TokenType::PRINT)) return printStatement();
    if (match(TokenType::PRINTLN)) return printlnStatement();
    if (match(TokenType::FUNC)) return funcStatement();

    // Fallback: try parsing as an expression statement
    ExprPtr expr = expression();  // or call your expression parser
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
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

StmtPtr Parser::printlnStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '('.");
    ExprPtr value = expression();
    consume(TokenType::RIGHT_PAREN, "Expected ')'.");
    consume(TokenType::SEMICOLON, "Expected ';'.");
    return std::make_shared<PrintlnStmt>(value);
}

ExprPtr Parser::expression() {
    ExprPtr left = term();

    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        char op = previous().lexeme[0];
        ExprPtr right = term();
        left = std::make_shared<BinaryExpr>(left, op, right);
    }

    return left;
}

ExprPtr Parser::term() {
    ExprPtr left = factor();

    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
        char op = previous().lexeme[0];
        ExprPtr right = factor();
        left = std::make_shared<BinaryExpr>(left, op, right);
    }

    return left;
}

ExprPtr Parser::factor() {
    if (match(TokenType::NUMBER)) {
        return std::make_shared<LiteralExpr>(std::stoi(previous().lexeme));
    }
    else if (match(TokenType::STRING)) {
        return std::make_shared<StringExpr>(previous().lexeme);
    }
    else if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().lexeme;

        // Check for function call
        if (match(TokenType::LEFT_PAREN)) {
            consume(TokenType::RIGHT_PAREN, "Expected ')' after function call.");
            return std::make_shared<CallExpr>(name);
        }

        return std::make_shared<VariableExpr>(name);
    }
    else if (match(TokenType::LEFT_PAREN)) {
        ExprPtr expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')'.");
        return expr;
    }

    throw std::runtime_error("Invalid expression.");
}

StmtPtr Parser::funcStatement() {
    consume(TokenType::IDENTIFIER, "Expected function name.");
    std::string name = previous().lexeme;

    consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");
    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expected '{' before function body.");

    std::vector<StmtPtr> body;

    while (!match(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        body.push_back(statement());
    }

    return std::make_shared<FuncStmt>(name, body);
}


std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> stmts;
    while (!isAtEnd()) {
        stmts.push_back(statement());
    }
    return stmts;
}
