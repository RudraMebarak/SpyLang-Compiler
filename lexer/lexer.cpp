#include "lexer.h"
#include <cctype>
#include <unordered_map>
#include <iostream>

// ---------- Keyword map ----------
static std::unordered_map<std::string, TokenType> keywords = {
    {"exhibit_int", TOKEN_INT},
    {"exhibit_float", TOKEN_FLOAT},
    {"exhibit_char", TOKEN_CHAR},
    {"suspect", TOKEN_IF},
    {"otherwise", TOKEN_ELSE},
    {"loop", TOKEN_WHILE},
    {"show", TOKEN_PRINT}
};

// ---------- Constructor ----------
Lexer::Lexer(const std::string& source)
    : src(source), pos(0), currentChar(0)
{
    currentChar = src[pos];
}

// ---------- Helper: advance one character ----------
void Lexer::advance() {
    pos++;
    if (pos < src.length())
        currentChar = src[pos];
    else
        currentChar = '\0'; // EOF marker
}

// ---------- Helper: lookahead ----------
char Lexer::peek() const {
    if (pos + 1 < src.length())
        return src[pos + 1];
    return '\0';
}

// ---------- Helper: skip whitespace ----------
void Lexer::skipWhitespace() {
    while (currentChar != '\0' && std::isspace(currentChar)) {
        advance();
    }
}

// ---------- Read identifier or keyword ----------
Token Lexer::identifier() {
    std::string value;

    while (std::isalnum(currentChar) || currentChar == '_') {
        value.push_back(currentChar);
        advance();
    }

    // Check if keyword
    if (keywords.count(value)) {
        return {keywords[value], value};
    }

    return {TOKEN_ID, value};
}

// ---------- Read number ----------
Token Lexer::number() {
    std::string value;

    while (std::isdigit(currentChar)) {
        value.push_back(currentChar);
        advance();
    }

    return {TOKEN_NUMBER, value};
}

// ---------- Read string literal ----------
Token Lexer::stringLiteral() {
    std::string value;

    advance(); // skip opening quote "

    while (currentChar != '\0' && currentChar != '"') {
        value.push_back(currentChar);
        advance();
    }

    if (currentChar == '\0') {
        return {TOKEN_INVALID, "Unterminated string"};
    }

    advance(); // skip closing quote "
    return {TOKEN_STRING, value};
}

// ---------- Main lexer function ----------
Token Lexer::getNextToken() {

    while (currentChar != '\0') {

        // Ignore whitespace
        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        // Identifier or keyword
        if (std::isalpha(currentChar) || currentChar == '_') {
            return identifier();
        }

        // Number
        if (std::isdigit(currentChar)) {
            return number();
        }

        // String literal
        if (currentChar == '"') {
            return stringLiteral();
        }

        // Operators & symbols
        switch (currentChar) {

            case '+': advance(); return {TOKEN_PLUS, "+"};
            case '-': advance(); return {TOKEN_MINUS, "-"};
            case '*': advance(); return {TOKEN_MUL, "*"};
            case '/': advance(); return {TOKEN_DIV, "/"};

            case '=':
                if (peek() == '=') {
                    advance(); advance();
                    return {TOKEN_EQ, "=="};
                }
                advance();
                return {TOKEN_ASSIGN, "="};

            case '<':
                advance();
                return {TOKEN_LT, "<"};

            case '>':
                advance();
                return {TOKEN_GT, ">"};

            case '(':
                advance();
                return {TOKEN_LPAREN, "("};

            case ')':
                advance();
                return {TOKEN_RPAREN, ")"};

            case '{':
                advance();
                return {TOKEN_LBRACE, "{"};

            case '}':
                advance();
                return {TOKEN_RBRACE, "}"};

            case ';':
                advance();
                return {TOKEN_SEMI, ";"};

            default: {
                char bad = currentChar;
                advance();
                return {TOKEN_INVALID, std::string(1, bad)};
            }
        }
    }

    return {TOKEN_EOF, ""};
}
