#pragma once
#include <string>
#include <memory>

struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : public Expr {
    std::string value;
    NumberExpr(const std::string& value) : value(value) {}
};

struct IdentifierExpr : public Expr {
    std::string name;
    IdentifierExpr(const std::string& name) : name(name) {}
};

struct BinaryExpr : public Expr {
    std::unique_ptr<Expr> left;
    std::string op;
    std::unique_ptr<Expr> right;

    BinaryExpr(std::unique_ptr<Expr> left, const std::string& op, std::unique_ptr<Expr> right)
        : left(std::move(left)), op(op), right(std::move(right)) {}
};
