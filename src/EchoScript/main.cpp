#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: escript <file.es>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Lexer lexer(source);
    auto tokens = lexer.tokenize();

    Parser parser(tokens);
    auto statements = parser.parse();

    std::unordered_map<std::string, int> env;

    for (auto& stmt : statements) {
        stmt->execute(env);
    }

    return 0;
}
