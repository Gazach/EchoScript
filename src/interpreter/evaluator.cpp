#include "evaluator.hpp"
#include "../AST/expr.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

int evalExpr(const Expr* expr, const Environment& env) {
    if (auto number = dynamic_cast<const NumberExpr*>(expr)) {
        // Convert string number to int
        return std::stoi(number->value);
    } 
    else if (auto variable = dynamic_cast<const IdentifierExpr*>(expr)) {
        return env.get(variable->name);
    } 
    else if (auto binary = dynamic_cast<const BinaryExpr*>(expr)) {
        int left = evalExpr(binary->left.get(), env);
        int right = evalExpr(binary->right.get(), env);

        if (binary->op == "+") {
            return left + right;
        } else if (binary->op == "-") {
            return left - right;
        } else if (binary->op == "*") {
            return left * right;
        } else if (binary->op == "/") {
            return left / right;
        } else {
            throw std::runtime_error("Unknown binary operator: " + binary->op);
        }
    }

    throw std::runtime_error("Unknown expression type.");
}
void evaluate(const std::vector<StmtPtr>& statements, Environment& env) {
    for (const auto& stmt : statements) {
        if (auto letStmt = dynamic_cast<LetStmt*>(stmt.get())) {
            int value = evalExpr(letStmt->value.get(), env);
            env.set(letStmt->name, value);
        } else if (auto printStmt = dynamic_cast<PrintStmt*>(stmt.get())) {
            int value = evalExpr(printStmt->value.get(), env);
            std::cout << value << std::endl;
        }
    }
}
