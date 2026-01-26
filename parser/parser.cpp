#include "parser.h"
#include "../lexer/lexer.h"
#include <iostream>
#include <cstdlib>
#include<string>

Lexer* lexer = nullptr;
Token currentToken;

void advance();
void expect(TokenType type);
void syntaxError();

void advance() {
    currentToken = lexer->getNextToken();
}

void expect(TokenType type) {
    if (currentToken.type == type)
        advance();
    else
        syntaxError();
}

void syntaxError() {
    std::cerr << "Syntax error at line "
              << currentToken.line
              << ", column "
              << currentToken.column
              << ": " << std::endl;
    exit(1);
}

void parse(const std::string& source) {
    lexer = new Lexer(source);
    advance();          // load first token
    parseProgram();
}

void parseProgram() {
    parseStatementList();
    expect(TOKEN_EOF);
}

void parseStatementList() {
    while (currentToken.type != TOKEN_EOF) {
        parseStatement();
    }
}
 void parseStatement()
 {
    std::cout<<"statement";
    advance();
 }

