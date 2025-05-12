#pragma once
#include <variant>
#include <string>
#include <stdexcept>
#include <sstream>


struct Value {
    using ValueData = std::variant<int, double, std::string, bool, char>;

    ValueData data;

    Value() : data(0) {}  // Default constructor (int)
    Value(int v) : data(v) {}
    Value(const std::string& v) : data(v) {}
    Value(bool b) : data(b) {}
    explicit Value(char c) : data(c) {}
    Value(double d) : data(d) {}

    // Accessor functions
    int asInt() const {
        if (std::holds_alternative<int>(data)) {
            return std::get<int>(data);
        }
        throw std::runtime_error("Value is not an integer");
    }
    
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
    double asDouble() const {
        if (std::holds_alternative<double>(data)) return std::get<double>(data);
        if (std::holds_alternative<int>(data)) return std::get<int>(data);
        throw std::runtime_error("Value is not a number");
    }

    std::string toString() const {
        if (std::holds_alternative<double>(data)) {
            double val = std::get<double>(data);
            std::ostringstream oss;

            // Check if value is an integer but stored as double
            if (val == static_cast<int64_t>(val)) {
                oss << static_cast<int64_t>(val) << ".0";
            }
            else {
                oss << val;
                std::string s = oss.str();

                // Remove trailing zeros for non-scientific notation numbers
                if (s.find('e') == std::string::npos) {
                    size_t dot_pos = s.find('.');
                    if (dot_pos != std::string::npos) {
                        // Trim trailing zeros
                        s = s.substr(0, s.find_last_not_of('0') + 1);
                        // Ensure we don't leave a dangling decimal point
                        if (s.back() == '.') {
                            s += '0';
                        }
                    }
                    return s;
                }
                return s;
            }
            return oss.str();
        }
        if (std::holds_alternative<char>(data)) {
            return std::string(1, std::get<char>(data));
        }
        if (std::holds_alternative<std::string>(data)) {
            return std::get<std::string>(data);
        }
        if (std::holds_alternative<bool>(data)) {
            return std::get<bool>(data) ? "true" : "false";
        }
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
