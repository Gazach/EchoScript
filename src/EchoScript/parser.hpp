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
    const Token& consume(TokenType type, const std::string& message);


    bool check(TokenType type);
    std::vector<StmtPtr> block();

    // Statement parsing functions
    StmtPtr statement();
    StmtPtr letStatement();
    StmtPtr printStatement();
    StmtPtr printlnStatement();
    StmtPtr funcStatement();

    // Expression parsing functions
    ExprPtr expression();
    ExprPtr term();
    ExprPtr factor();
};
