#include "lexer.hpp"
#include "parser.hpp"
#include "value.hpp"
#include "ast.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iostream>
#include <string>

bool has_es_extension(const std::string& filename) {
    // Check if the file ends with ".es"
    return filename.size() > 3 && filename.substr(filename.size() - 3) == ".es";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: escript <file.es>\n";
        return 1;
    }

    std::string filename = argv[1];

    // Check if the file has a .es extension
    if (!has_es_extension(filename)) {
        std::cerr << "EchoScript Error: The file must have a .es extension.\n";
        return 1;
    }

    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    // Tokenize and parse the source code
    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto statements = parser.parse();

    std::unordered_map<std::string, Value> env;
    std::unordered_map<std::string, FuncStmt*> funcs;

    // Execute each statement
    for (const auto& stmt : statements) {
        try {
            stmt->execute(env, funcs);  // Pass funcs to execute method
        }
        catch (const std::exception& e) {
            std::cerr << "Error during execution: " << e.what() << std::endl;
            return 1;  // Stop execution on error
        }
    }

    return 0;
}
