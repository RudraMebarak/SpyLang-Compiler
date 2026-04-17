#include "parser.h"
#include "../lexer/lexer.h"
#include <iostream>
#include <cstdlib>
#include<string>

Lexer* lexer = nullptr;
Token currentToken;
int indent = 0;

void printIndent() {
    for (int i = 0; i < indent; i++) std::cout << "  ";
}
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
 void parseStatement() {
    printIndent();
    std::cout << "Parsing Statement\n";

    indent++;

    if (currentToken.type == TOKEN_INT || 
        currentToken.type == TOKEN_FLOAT) {
        parseDeclaration();
    }
    else if (currentToken.type == TOKEN_ID) {
        parseAssignment();
    } 
    else if (currentToken.type == TOKEN_IF) {
        parseIfStatement();
    } 
    else {
        syntaxError();
    }

    indent--;
}

void parseDeclaration() {
    printIndent();
    std::cout << "Declaration\n";

    indent++;

    // Type
    if (currentToken.type == TOKEN_INT || 
        currentToken.type == TOKEN_FLOAT) {
        
        printIndent();
        std::cout << "Type: " << currentToken.lexeme << "\n";
        advance();
    } 
    else {
        syntaxError();
    }

    // Declarator list
    parseDeclaratorList();

    expect(TOKEN_SEMI);

    indent--;
}

void parseDeclaratorList() {
    printIndent();
    std::cout << "DeclaratorList\n";

    indent++;

    // First ID
    if (currentToken.type == TOKEN_ID) {
        printIndent();
        std::cout << "Var: " << currentToken.lexeme << "\n";
        advance();
    } 
    else {
        syntaxError();
    }

    // Additional IDs: , ID , ID ...
    while (currentToken.type == TOKEN_COMMA) {
        advance(); // consume ','

        if (currentToken.type == TOKEN_ID) {
            printIndent();
            std::cout << "Var: " << currentToken.lexeme << "\n";
            advance();
        } 
        else {
            syntaxError();
        }
    }

    indent--;
}


void parseAssignment() {
    printIndent();
    std::cout << "Assignment\n";

    indent++;

    printIndent();
    std::cout << "ID: " << currentToken.lexeme << "\n";
    expect(TOKEN_ID);

    expect(TOKEN_ASSIGN);

    parseExpression();

    expect(TOKEN_SEMI);

    indent--;
}

void parseIfStatement() {
    printIndent();
    std::cout << "IfStatement\n";

    indent++;

    expect(TOKEN_IF);

    expect(TOKEN_LPAREN);
    parseExpression();
    expect(TOKEN_RPAREN);

    printIndent();
    std::cout << "Then:\n";
    indent++;
    parseStatement();
    indent--;

    if (currentToken.type == TOKEN_ELSE) {
        printIndent();
        std::cout << "Else:\n";
        advance();

        indent++;
        parseStatement();
        indent--;
    }

    indent--;
}

void parseExpression() {
    printIndent();
    std::cout << "Expression\n";

    indent++;

    parseTerm();

    while (currentToken.type == TOKEN_PLUS || 
           currentToken.type == TOKEN_MINUS) {
        printIndent();
        std::cout << "Operator: " << currentToken.lexeme << "\n";
        advance();
        parseTerm();
    }

    indent--;
}

void parseTerm() {
    printIndent();
    std::cout << "Term\n";

    indent++;

    parseFactor();

    while (currentToken.type == TOKEN_MUL || 
           currentToken.type == TOKEN_DIV) {
        printIndent();
        std::cout << "Operator: " << currentToken.lexeme << "\n";
        advance();
        parseFactor();
    }

    indent--;
}

void parseFactor() {
    printIndent();
    std::cout << "Factor: " << currentToken.lexeme << "\n";

    if (currentToken.type == TOKEN_ID || 
        currentToken.type == TOKEN_NUMBER) {
        advance();
    } 
    else if (currentToken.type == TOKEN_LPAREN) {
        advance();
        parseExpression();
        expect(TOKEN_RPAREN);
    } 
    else {
        syntaxError();
    }
}

