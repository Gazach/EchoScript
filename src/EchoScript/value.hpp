#pragma once
#include <variant>
#include <string>

struct Value {
    using ValueData = std::variant<int, std::string>;

    ValueData data;

    Value() : data(0) {}  // Default constructor (int)
    Value(int v) : data(v) {}
    Value(const std::string& v) : data(v) {}

    // Accessor functions
    int asInt() const {
        if (std::holds_alternative<int>(data)) {
            return std::get<int>(data);
        }
        throw std::runtime_error("Value is not an integer");
    }

    std::string toString() const {
        if (std::holds_alternative<std::string>(data)) {
            return std::get<std::string>(data);
        }
        return std::to_string(asInt()); // default to integer conversion
    }

    bool isInt() const {
        return std::holds_alternative<int>(data);
    }
};
