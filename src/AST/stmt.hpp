#pragma once
#include <memory>
#include <string>
#include "expr.hpp"

struct Stmt {
    virtual ~Stmt() = default;
};

using StmtPtr = std::unique_ptr<Stmt>;

struct LetStmt : public Stmt {
    std::string name;
    std::unique_ptr<Expr> value;

    LetStmt(const std::string& name, std::unique_ptr<Expr> value)
        : name(name), value(std::move(value)) {}
};

struct PrintStmt : public Stmt {
    std::unique_ptr<Expr> value;

    PrintStmt(std::unique_ptr<Expr> value)
        : value(std::move(value)) {}
};
