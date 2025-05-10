#pragma once
#include <string>
#include <memory>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <vector>
#include "value.hpp"
// Forward declaration
struct Expression;
struct Statement;

using ExprPtr = std::shared_ptr<Expression>;
using StmtPtr = std::shared_ptr<Statement>;


struct Expression {
    virtual ~Expression() = default;
    virtual Value evaluate(std::unordered_map<std::string, Value>& env) = 0;
};

struct Statement {
    virtual ~Statement() = default;
    virtual void execute(std::unordered_map<std::string, Value>& env) = 0;
};

struct VariableExpr : Expression {
    std::string name;
    VariableExpr(const std::string& name) : name(name) {}

    Value evaluate(std::unordered_map<std::string, Value>& env) override {
        if (env.find(name) == env.end())
            throw std::runtime_error("Undefined variable: " + name);
        return env[name];
    }
};


struct LiteralExpr : Expression {
    int value;
    LiteralExpr(int val) : value(val) {}

    Value evaluate(std::unordered_map<std::string, Value>&) override {
        return Value(value);
    }
};

struct LetStmt : Statement {
    std::string name;
    ExprPtr value;

    LetStmt(const std::string& name, ExprPtr value) : name(name), value(value) {}

    void execute(std::unordered_map<std::string, Value>& env) override {
        env[name] = value->evaluate(env); // ✅ Works: both are Value now
    }
};

struct PrintStmt : public Statement {
    ExprPtr value;

    PrintStmt(ExprPtr value) : value(value) {}

    void execute(std::unordered_map<std::string, Value>& env) override {
        Value val = value->evaluate(env);
        std::cout << val.toString();  // Print without newline
    }
};

struct PrintlnStmt : public Statement {
    ExprPtr value;

    PrintlnStmt(ExprPtr value) : value(value) {}

    void execute(std::unordered_map<std::string, Value>& env) override {
        Value val = value->evaluate(env);
        std::cout << val.toString() << std::endl;  // Print with newline
    }
};

struct BinaryExpr : Expression {
    ExprPtr left;
    ExprPtr right;
    char op;

    BinaryExpr(ExprPtr l, char o, ExprPtr r) : left(l), op(o), right(r) {}

    Value evaluate(std::unordered_map<std::string, Value>& env) override {
        Value lhs = left->evaluate(env);
        Value rhs = right->evaluate(env);

        if (std::holds_alternative<std::string>(lhs.data) || std::holds_alternative<std::string>(rhs.data)) {
            return Value(lhs.toString() + rhs.toString());
        }

        int l = lhs.asInt();
        int r = rhs.asInt();

        switch (op) {
        case '+': return Value(l + r);
        case '-': return Value(l - r);
        case '*': return Value(l * r);
        case '/': return Value(l / r);
        default: throw std::runtime_error("Unknown operator.");
        }
    }
};


struct StringExpr : Expression {
    std::string value;
    StringExpr(const std::string& val) : value(val) {}

    Value evaluate(std::unordered_map<std::string, Value>&) override {
        return Value(value);
    }
};

