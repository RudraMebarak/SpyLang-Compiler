#include "lexer.h"
#include <cctype>
#include <unordered_map>

// --------------------------------------------
// Keyword table (lexeme → TokenType)
// --------------------------------------------
static const std::unordered_map<std::string, TokenType> keywords = {
    {"exhibit_int",   TOKEN_INT},
    {"exhibit_float", TOKEN_FLOAT},
    {"exhibit_char",  TOKEN_CHAR},
    {"suspect",    TOKEN_IF},
    {"otherwise",  TOKEN_ELSE},
    {"investigate_till", TOKEN_WHILE},
    {"showcase", TOKEN_PRINT}
};

// --------------------------------------------
// Constructor
// --------------------------------------------
Lexer::Lexer(const std::string& src)
    : source(src),
      position(0),
      currentChar('\0'),
      line(1),
      column(0)
{
    if (!source.empty()) {
        currentChar = source[position];
    }
}

// --------------------------------------------
// Advance one character
// --------------------------------------------
void Lexer::advance() {
    if (currentChar == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }

    position++;

    if (position < source.size())
        currentChar = source[position];
    else
        currentChar = '\0';
}

// --------------------------------------------
// Lookahead
// --------------------------------------------
char Lexer::peek() const {
    if (position + 1 < source.size())
        return source[position + 1];
    return '\0';
}

// --------------------------------------------
// Skip whitespace
// --------------------------------------------
void Lexer::skipWhitespace() {
    while (currentChar != '\0' && std::isspace(currentChar)) {
        advance();
    }
}

// --------------------------------------------
// Identifier or keyword
// --------------------------------------------
Token Lexer::readIdentifierOrKeyword() {
    int startColumn = column;
    std::string lexeme;

    while (std::isalnum(currentChar) || currentChar == '_') {
        lexeme.push_back(currentChar);
        advance();
    }

    auto it = keywords.find(lexeme);
    if (it != keywords.end()) {
        return {it->second, lexeme, line, startColumn};
    }

    return {TOKEN_ID, lexeme, line, startColumn};
}

// --------------------------------------------
// Number literal
// --------------------------------------------
Token Lexer::readNumber() {
    int startColumn = column;
    std::string lexeme;

    while (std::isdigit(currentChar)) {
        lexeme.push_back(currentChar);
        advance();
    }

    return {TOKEN_NUMBER, lexeme, line, startColumn};
}

// --------------------------------------------
// Operator or delimiter
// --------------------------------------------
Token Lexer::readOperatorOrDelimiter() {
    int startColumn = column;
    char ch = currentChar;

    switch (ch) {
        case '+': advance(); return {TOKEN_PLUS, "+", line, startColumn};
        case '-': advance(); return {TOKEN_MINUS, "-", line, startColumn};
        case '*': advance(); return {TOKEN_MUL, "*", line, startColumn};
        case '/': advance(); return {TOKEN_DIV, "/", line, startColumn};

        case '=':
            if (peek() == '=') {
                advance(); advance();
                return {TOKEN_EQ, "==", line, startColumn};
            }
            advance();
            return {TOKEN_ASSIGN, "=", line, startColumn};

        case '<': advance(); return {TOKEN_LT, "<", line, startColumn};
        case '>': advance(); return {TOKEN_GT, ">", line, startColumn};

        case ',': advance(); return {TOKEN_COMMA, ",", line, startColumn};

        case '(':
            advance(); return {TOKEN_LPAREN, "(", line, startColumn};
        case ')':
            advance(); return {TOKEN_RPAREN, ")", line, startColumn};
        case '{':
            advance(); return {TOKEN_LBRACE, "{", line, startColumn};
        case '}':
            advance(); return {TOKEN_RBRACE, "}", line, startColumn};
        case ';':
            advance(); return {TOKEN_SEMI, ";", line, startColumn};

        default:
            advance();
            return {TOKEN_INVALID, std::string(1, ch), line, startColumn};
    }
}

// --------------------------------------------
// Main lexer driver
// --------------------------------------------
Token Lexer::getNextToken() {

    while (currentChar != '\0') {

        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        if (std::isalpha(currentChar) || currentChar == '_') {
            return readIdentifierOrKeyword();
        }

        if (std::isdigit(currentChar)) {
            return readNumber();
        }

        return readOperatorOrDelimiter();
    }

    return {TOKEN_EOF, "", line, column};
}
