#include "parser.hpp"
#include "ast.hpp" 
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

bool Parser::check(TokenType type) {
    // Don't run off the end
    if (isAtEnd()) return false;
    return peek().type == type;
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
    if (match(TokenType::RETURN)) return returnStatement();

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
    if (match(TokenType::TRUE)) {
        return std::make_shared<BooleanExpr>(true);
    }
    if (match(TokenType::FALSE)) {
        return std::make_shared<BooleanExpr>(false);
    }
    if (match(TokenType::CHAR)) {
        std::string charVal = previous().lexeme;
        return std::make_shared<CharExpr>(charVal[0]);
    }
    if (match(TokenType::FLOAT)) {
        double d = std::stod(previous().lexeme);
        return std::make_shared<LiteralExpr>(Value(d));
    }
    if (match(TokenType::NUMBER)) {
        int i = std::stoi(previous().lexeme);
        return std::make_shared<LiteralExpr>(Value(i));
    }
    else if (match(TokenType::STRING)) {
        return std::make_shared<StringExpr>(previous().lexeme);
    }
    else if (match(TokenType::IDENTIFIER)) {
        std::string name = previous().lexeme;

        // Did the user type '(' immediately after the identifier?
        if (match(TokenType::LEFT_PAREN)) {
            std::vector<ExprPtr> args;

            // If the next token isn’t ')', we have at least one argument
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    args.push_back(expression());
                } while (match(TokenType::COMMA));
            }

            // Now we must close the call
            consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");

            // Return a CallExpr carrying name + the args vector
            return std::make_shared<CallExpr>(name, args);
        }

        // Otherwise it’s just a variable
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
    // we’ve already matched TokenType::FUNC
    Token nameTok = consume(TokenType::IDENTIFIER, "Expected function name.");
    std::string name = nameTok.lexeme;

    consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");
    std::vector<std::string> params;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            Token param = consume(TokenType::IDENTIFIER, "Expected parameter name.");
            params.push_back(param.lexeme);
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");

    consume(TokenType::LEFT_BRACE, "Expected '{' before function body.");
    std::vector<StmtPtr> body;
    while (!match(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        body.push_back(statement());
    }

    return std::make_shared<FuncStmt>(name, params, body);
}


StmtPtr Parser::returnStatement() {
    ExprPtr value = expression();
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return std::make_shared<ReturnStmt>(value);
}


std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> stmts;
    while (!isAtEnd()) {
        stmts.push_back(statement());
    }
    return stmts;
}
