#pragma once
#include "token.hpp"
#include "ast.hpp"

class Parser {
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<StmtPtr> parse();

private:
    const std::vector<Token>& tokens;
    int current = 0;

    bool isAtEnd();
    const Token& peek();
    const Token& advance();
    const Token& previous();
    bool match(TokenType type);
    void consume(TokenType type, const std::string& error);

    StmtPtr statement();
    StmtPtr letStatement();
    StmtPtr printStatement();
    ExprPtr expression();
    ExprPtr term();
    ExprPtr factor();

};
