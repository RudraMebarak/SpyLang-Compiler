#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "../lexer/lexer.h"

// global parser state
extern Lexer* lexer;
extern Token currentToken;

void parse(const std::string& source);
void parseProgram();

void parseStatementList();

void parseStatement();

void parseDeclaration();
void parseDeclaratorList();

void parseAssignment();

void parsePrintStatement();

void parseIfStatement();
void parseElsePart();

void parseWhileStatement();

void parseBlock();

void parseExpression();
void parseTerm();
void parseExpressiont();
void parseFactor();
void parseTermt();

void parseCondition();
void parseRelOp();

#endif
