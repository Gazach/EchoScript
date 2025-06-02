#pragma once
#include <string>
#include <cctype> // for isspace

class scanner {
public:
    scanner(const std::string& input)
        : input(input), pos(0) {}

    char currentChar() const {
        return pos < input.size() ? input[pos] : '\0';
    }

    char advance() {
        return pos < input.size() ? input[pos++] : '\0';
    }

    void skipWhitespace() {
        while (std::isspace(currentChar())) advance();
    }

    bool isEOF() const {
        return pos >= input.size();
    }

    size_t getPosition() const { return pos; }

    template <typename Predicate>
    std::string readWhile(Predicate predicate) {
        std::string result;
        while (!isEOF() && predicate(currentChar())) {
            result += advance();
        }
        return result;
    }

private:
    std::string input;
    size_t pos;
};
