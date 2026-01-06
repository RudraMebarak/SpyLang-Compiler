#ifndef LEXER_H
#define LEXER_H

#include <string>
#include "token.h"

class Lexer {
public:
    // Construct lexer from source code string
    explicit Lexer(const std::string& source);

    // Return the next token from the input stream
    Token getNextToken();

private:
    // ---------- Input state ----------
    std::string source;   // full source code
    size_t position;      // current index in source
    char currentChar;     // current character

    // ---------- Location tracking ----------
    int line;
    int column;

    // ---------- Core helpers ----------
    void advance();            // move to next character
    char peek() const;         // lookahead without consuming
    void skipWhitespace();     // ignore spaces, tabs, newlines

    // ---------- Token readers ----------
    Token readIdentifierOrKeyword();
    Token readNumber();
    Token readString();
    Token readOperatorOrDelimiter();
};

#endif
