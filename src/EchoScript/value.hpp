#pragma once
#include <variant>
#include <string>

class Value {
public:
    std::variant<int, std::string> data;

    // Default constructor
    Value() : data(0) {}  // Default to int 0, or you could use an empty string: Value("") for a default string

    Value(int v) : data(v) {}
    Value(const std::string& v) : data(v) {}

    std::string toString() const {
        if (std::holds_alternative<int>(data)) {
            return std::to_string(std::get<int>(data));
        }
        else {
            return std::get<std::string>(data);
        }
    }

    int asInt() const {
        if (std::holds_alternative<int>(data)) {
            return std::get<int>(data);
        }
        throw std::runtime_error("Expected int");
    }

    std::string asString() const {
        if (std::holds_alternative<std::string>(data)) {
            return std::get<std::string>(data);
        }
        throw std::runtime_error("Expected string");
    }
};
