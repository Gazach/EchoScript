#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer/lexer.hpp"    // umbrella header containing Scanner and tokenize
#include "lexer/token_recognition.hpp"    // for tokenTypeToString
#include "parser/parser.hpp"  // parser that builds AST

// Helper: read entire file into string
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./eslang <source>.es\n";
        return 1;
    }

    std::string filename = argv[1];
    // Ensure it has .es extension
    if (filename.size() < 4 || filename.substr(filename.size() - 3) != ".es") {
        std::cerr << "Error: Only .es files are supported.\n";
        return 1;
    }

    try {
        // Read source code
        std::string source = readFile(filename);

        // Lexical analysis (tokenization)
        Lexer lexer(source);
        auto tokens = lexer.tokenize();

        // DEBUG: Print tokens
        std::cout << "Tokens:\n";
        for (const auto& token : tokens) {
            std::cout << tokenTypeToString(token.type)
                      << " (" << token.value << ")\n";
        }

        // Parsing into AST
        Parser parser(tokens);
        auto statements = parser.parse();

        std::cout << "Parsed " << statements.size() << " statement(s).\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}