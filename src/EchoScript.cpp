#include <iostream>
#include "lexer/lexer.hpp"
#include "lexer/token_utils.hpp"

int main() {
    std::string code = R"(
        let x = 5;
        let y = 10;
        print(x + y);
    )";

    Lexer lexer(code);
    auto tokens = lexer.tokenize();

    for (const auto& token : tokens) {
        std::cout << "Token: "
                << tokenTypeToString(token.type)
                << ", Value: " << token.value
                << "\n";
    }

    return 0;
}
