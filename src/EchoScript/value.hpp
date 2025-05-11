#pragma once
#include <variant>
#include <string>

struct Value {
    using ValueData = std::variant<int, std::string, bool, char>;

    ValueData data;

    Value() : data(0) {}  // Default constructor (int)
    Value(int v) : data(v) {}
    Value(const std::string& v) : data(v) {}
    Value(bool b) : data(b) {}
    explicit Value(char c) : data(c) {}

    // Accessor functions
    int asInt() const {
        if (std::holds_alternative<int>(data)) {
            return std::get<int>(data);
        }
        throw std::runtime_error("Value is not an integer");
    }
    // Add accessors
    bool asBool() const {
        if (std::holds_alternative<bool>(data)) {
            return std::get<bool>(data);
        }
        throw std::runtime_error("Value is not a boolean");
    }

    char asChar() const {
        if (std::holds_alternative<char>(data)) {
            return std::get<char>(data);
        }
        throw std::runtime_error("Value is not a character");
    }
    std::string toString() const {
        // Add character case first
        if (std::holds_alternative<char>(data)) {
            return std::string(1, std::get<char>(data));
        }
        if (std::holds_alternative<std::string>(data)) {
            return std::get<std::string>(data);
        }
        if (std::holds_alternative<bool>(data)) {
            return std::get<bool>(data) ? "true" : "false";
        }
        // Default to integer conversion
        return std::to_string(std::get<int>(data));
    }

    int asNumber() const {
        if (std::holds_alternative<int>(data)) return std::get<int>(data);
        if (std::holds_alternative<bool>(data)) return std::get<bool>(data) ? 1 : 0;
        if (std::holds_alternative<char>(data)) return static_cast<int>(std::get<char>(data));
        throw std::runtime_error("Value is not numeric");
    }
    bool isInt() const {
        return std::holds_alternative<int>(data);
    }

    bool isChar() const {
        return std::holds_alternative<char>(data);
    }

    bool isBool() const {
        return std::holds_alternative<bool>(data);
    }
};
