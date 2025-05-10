#pragma once

#include <string>
#include <memory>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "value.hpp"
#include "return_exception.hpp" 

struct FuncStmt;

struct Expression;
struct Statement;

using ExprPtr = std::shared_ptr<Expression>;
using StmtPtr = std::shared_ptr<Statement>;

// ast.hpp
struct Expression {
    virtual ~Expression() = default;
    virtual Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) = 0;
};

struct Statement {
    virtual ~Statement() = default;
    virtual void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) = 0;
};


struct ExpressionStmt : Statement {
    ExprPtr expr;
    ExpressionStmt(ExprPtr e) : expr(e) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        expr->evaluate(env, funcs);  
    }

};


// Variable Expression (e.g., x)
struct VariableExpr : Expression {
    std::string name;
    VariableExpr(const std::string& name) : name(name) {}

    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        if (env.find(name) == env.end())
            throw std::runtime_error("Undefined variable: " + name);
        return env[name];
    }
};


// Literal Expression (e.g., 42)
struct LiteralExpr : Expression {
    int value;
    LiteralExpr(int val) : value(val) {}

    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        return Value(value);
    }
};


// Statement for Variable Assignment (e.g., let x = 5)
struct LetStmt : Statement {
    std::string name;
    ExprPtr value;

    LetStmt(const std::string& name, ExprPtr value) : name(name), value(value) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        env[name] = value->evaluate(env, funcs);  // Fixed: include both env and funcs
    }

};

// Statement for Printing (e.g., print(x))
struct PrintStmt : public Statement {
    ExprPtr value;

    PrintStmt(ExprPtr value) : value(value) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        Value val = value->evaluate(env, funcs);  // Fixed: added `funcs`
        std::cout << val.toString() << std::endl;  // Print with newline
    }

};

// Statement for Printing with Newline (e.g., println(x))
struct PrintlnStmt : public Statement {
    ExprPtr value;

    PrintlnStmt(ExprPtr value) : value(value) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        Value val = value->evaluate(env, funcs);
        std::cout << val.toString() << std::endl;
    }

};

// Binary Expression (e.g., x + y)
struct BinaryExpr : Expression {
    ExprPtr left;
    ExprPtr right;
    char op;

    BinaryExpr(ExprPtr l, char o, ExprPtr r) : left(l), op(o), right(r) {}

    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        Value lhs = left->evaluate(env, funcs);
        Value rhs = right->evaluate(env, funcs);

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


// String expression
struct StringExpr : Expression {
    std::string value;
    StringExpr(const std::string& val) : value(val) {}


    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        return Value(value);
    }
};

struct FuncStmt : Statement {
    std::string name;
    std::vector<StmtPtr> body;

    FuncStmt(const std::string& name, const std::vector<StmtPtr>& body)
        : name(name), body(body) {}

    // Corrected `execute` method to match the base class signature
    void execute(std::unordered_map<std::string, Value>& env,
                 std::unordered_map<std::string, FuncStmt*>& funcs) override {
        funcs[name] = this;  // Store FuncStmt* directly in funcs
    }

    Value call(std::unordered_map<std::string, Value>& env,
               std::unordered_map<std::string, FuncStmt*>& funcs) {
        std::unordered_map<std::string, Value> localEnv = env;

        try {
            for (auto& stmt : body) {  // Use the body stored in funcs
                stmt->execute(localEnv, funcs);  // Execute each statement in the body
            }
        }
        catch (const Value& returnVal) {
            return returnVal;  // Return value from function (or exit early)
        }

        return Value();  // Default return value (for now, could be void)
    }
};

    

// Function Call Expression (e.g., greet())
struct CallExpr : Expression {
    std::string name;

    CallExpr(const std::string& name) : name(name) {}

    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        auto it = funcs.find(name);
        if (it == funcs.end()) throw std::runtime_error("Function not defined: " + name);

        // Get the function definition (FuncStmt*)
        FuncStmt* func = it->second;

        // Try executing the function and handle the return exception
        try {
            // Call the function with the current environment and functions map
            return func->call(env, funcs);
        }
        catch (const ReturnException& ret) {
            // Catch the return exception and return the value from the function
            return ret.value;  // This will be the value returned by the function
        }
    }
};


struct ReturnStmt : public Statement {
    ExprPtr expr;

    ReturnStmt(ExprPtr expr) : expr(expr) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, FuncStmt*>& funcs) override {
        // Evaluate the return expression
        Value result = expr->evaluate(env, funcs);
        // Throw the custom return exception with the result
        throw ReturnException(result);
    }
};
