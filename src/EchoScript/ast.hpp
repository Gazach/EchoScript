#pragma once
#include <string>
#include <memory>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <vector>

// Forward declaration
struct Expression;
struct Statement;

using ExprPtr = std::shared_ptr<Expression>;
using StmtPtr = std::shared_ptr<Statement>;

struct Expression {
    virtual ~Expression() = default;
    virtual int evaluate(std::unordered_map<std::string, int>& env) = 0;
};

struct Statement {
    virtual ~Statement() = default;
    virtual void execute(std::unordered_map<std::string, int>& env) = 0;
};

struct VariableExpr : Expression {
    std::string name;
    VariableExpr(const std::string& name) : name(name) {}
    int evaluate(std::unordered_map<std::string, int>& env) override {
        if (env.find(name) != env.end()) return env[name];
        throw std::runtime_error("Undefined variable: " + name);
    }
};


struct LiteralExpr : Expression {
    int value;
    LiteralExpr(int val) : value(val) {}
    int evaluate(std::unordered_map<std::string, int>&) override {
        return value;
    }
};

struct LetStmt : Statement {
    std::string name;
    ExprPtr value;
    LetStmt(const std::string& name, ExprPtr value) : name(name), value(value) {}
    void execute(std::unordered_map<std::string, int>& env) override {
        env[name] = value->evaluate(env);
    }
};

struct PrintStmt : Statement {
    ExprPtr expr;
    PrintStmt(ExprPtr expr) : expr(expr) {}
    void execute(std::unordered_map<std::string, int>& env) override {
        std::cout << expr->evaluate(env) << std::endl;
    }
};
