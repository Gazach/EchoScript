#pragma once
#include "../AST/stmt.hpp"
#include <unordered_map>
#include <string>
#include <stdexcept>

class Environment {
public:
    std::unordered_map<std::string, int> variables;

    void set(const std::string& name, int value) {
        variables[name] = value;
    }

    int get(const std::string& name) const {
        auto it = variables.find(name);
        if (it != variables.end()) return it->second;
        throw std::runtime_error("Undefined variable: " + name);
    }
};

void evaluate(const std::vector<StmtPtr>& statements, Environment& env);

