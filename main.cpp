#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer/lexer.h"
#include "parser/parser.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {

    std::string source = readFile("input/test.dl");

    std::cout << "Starting parser...\n";

    parse(source);   // entry function you chose earlier

    std::cout << "Parsing completed successfully.\n";
    return 0;
}
