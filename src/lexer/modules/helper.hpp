#pragma once
#include <string>
#include <cctype>

class scanner {
public:
    scanner(const std::string& input)
        : input(input), pos(0), line(1){}

    char currentChar() const {
        return pos < input.size() ? input[pos] : '\0';
    }

    char advance() {
        if (pos < input.size()) {
            if (input[pos] == '\n') {
                ++line;
            }
            return input[pos++];
        }
        return '\0';
    }

    void skipWhitespace() {
        while (std::isspace(currentChar())) advance();
    }

    bool isEOF() const {
        return pos >= input.size();
    }

    int getLine() const {
        return line;
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
    int line;
};
