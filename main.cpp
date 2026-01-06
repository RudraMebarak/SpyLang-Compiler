#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/lexer.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {

    std::string source = readFile("input/test.dl");

    Lexer lexer(source);

    while (true) {
        Token token = lexer.getNextToken();

        std::cout << "Token("
                  << static_cast<int>(token.type) << ", \""
                  << token.lexeme << "\""
                  << ", line " << token.line
                  << ", col " << token.column
                  << ")\n";

        if (token.type == TokenType::TOKEN_EOF)
            break;
    }

    return 0;
}
