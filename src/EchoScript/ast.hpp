#pragma once
#include <string>
#include <memory>
#include <variant>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "value.hpp" // Assuming Value is defined in this header

struct Expression;
struct Statement;

using ExprPtr = std::shared_ptr<Expression>;
using StmtPtr = std::shared_ptr<Statement>;

struct Expression {
    virtual ~Expression() = default;

    // Change this method signature to match with the one in CallExpr
    virtual Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) = 0;

};

struct Statement {
    virtual ~Statement() = default;
    virtual void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) = 0;
};

struct ExpressionStmt : Statement {
    ExprPtr expr;
    ExpressionStmt(ExprPtr e) : expr(e) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        expr->evaluate(env, funcs);  // Pass both arguments
    }

};


// Variable Expression (e.g., x)
struct VariableExpr : Expression {
    std::string name;
    VariableExpr(const std::string& name) : name(name) {}

    // Now including both env and funcs in the method signature
    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        if (env.find(name) == env.end())
            throw std::runtime_error("Undefined variable: " + name);
        return env[name];
    }
};


// Literal Expression (e.g., 42)
struct LiteralExpr : Expression {
    int value;
    LiteralExpr(int val) : value(val) {}

    // Now including both env and funcs in the method signature
    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        return Value(value);
    }
};


// Statement for Variable Assignment (e.g., let x = 5)
struct LetStmt : Statement {
    std::string name;
    ExprPtr value;

    LetStmt(const std::string& name, ExprPtr value) : name(name), value(value) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        env[name] = value->evaluate(env, funcs);  // Fixed: include both env and funcs
    }

};

// Statement for Printing (e.g., print(x))
struct PrintStmt : public Statement {
    ExprPtr value;

    PrintStmt(ExprPtr value) : value(value) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        Value val = value->evaluate(env, funcs);  // Fixed: added `funcs`
        std::cout << val.toString() << std::endl;  // Print with newline
    }

};

// Statement for Printing with Newline (e.g., println(x))
struct PrintlnStmt : public Statement {
    ExprPtr value;

    PrintlnStmt(ExprPtr value) : value(value) {}

    void execute(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
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

    // Now including both env and funcs in the method signature
    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        Value lhs = left->evaluate(env, funcs);  // Pass funcs argument here
        Value rhs = right->evaluate(env, funcs); // Pass funcs argument here

        if (std::holds_alternative<std::string>(lhs.data) || std::holds_alternative<std::string>(rhs.data)) {
            return Value(lhs.toString() + rhs.toString());  // String concatenation
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


// String Expression (e.g., "hello")
struct StringExpr : Expression {
    std::string value;
    StringExpr(const std::string& val) : value(val) {}

    // Now including both env and funcs in the method signature
    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        return Value(value); // No need to use funcs in this case, so just pass env and funcs
    }
};


// Function Definition Statement (e.g., func greet() { ... })
struct FuncStmt : Statement {
    std::string name;
    std::vector<StmtPtr> body;

    FuncStmt(const std::string& name, const std::vector<StmtPtr>& body)
        : name(name), body(body) {
    }

    void execute(std::unordered_map<std::string, Value>&,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        funcs[name] = body;  // Register the function
    }


};

// Function Call Expression (e.g., greet())
struct CallExpr : Expression {
    std::string name;

    CallExpr(const std::string& name) : name(name) {}

    Value evaluate(std::unordered_map<std::string, Value>& env,
        std::unordered_map<std::string, std::vector<StmtPtr>>& funcs) override {
        auto it = funcs.find(name);
        if (it == funcs.end()) throw std::runtime_error("Function not defined: " + name);

        // Execute function body (new scope for function execution)
        std::unordered_map<std::string, Value> localEnv = env;
        for (auto& stmt : it->second) {
            stmt->execute(localEnv, funcs);  // Execute each statement in the function body
        }

        return Value();  // Return void for now (or an appropriate return value)
    }
};

