#include "lexer.h"
#include <cctype>
#include <unordered_map>

// --------------------------------------------------
// Keyword table (lexeme → token type)
// --------------------------------------------------
static const std::unordered_map<std::string, TokenType> keywords = {
    {"num", TokenType::NUM},
    {"suspect", TokenType::SUSPECT},
    {"otherwise", TokenType::OTHERWISE},
    {"loop", TokenType::LOOP},
    {"show", TokenType::SHOW}
};

// --------------------------------------------------
// Constructor
// --------------------------------------------------
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

// --------------------------------------------------
// Advance one character
// --------------------------------------------------
void Lexer::advance() {
    if (currentChar == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }

    position++;

    if (position < source.size()) {
        currentChar = source[position];
    } else {
        currentChar = '\0'; // EOF marker
    }
}

// --------------------------------------------------
// Lookahead without consuming
// --------------------------------------------------
char Lexer::peek() const {
    if (position + 1 < source.size()) {
        return source[position + 1];
    }
    return '\0';
}

// --------------------------------------------------
// Skip whitespace
// --------------------------------------------------
void Lexer::skipWhitespace() {
    while (currentChar != '\0' && std::isspace(currentChar)) {
        advance();
    }
}

// --------------------------------------------------
// Read identifier or keyword
// --------------------------------------------------
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

    return {TokenType::IDENTIFIER, lexeme, line, startColumn};
}

// --------------------------------------------------
// Read number literal
// --------------------------------------------------
Token Lexer::readNumber() {
    int startColumn = column;
    std::string lexeme;

    while (std::isdigit(currentChar)) {
        lexeme.push_back(currentChar);
        advance();
    }

    return {TokenType::NUMBER, lexeme, line, startColumn};
}

// --------------------------------------------------
// Read string literal (preserves spaces)
// --------------------------------------------------
Token Lexer::readString() {
    int startColumn = column;
    std::string lexeme;

    advance(); // skip opening "

    while (currentChar != '\0' && currentChar != '"') {
        lexeme.push_back(currentChar);
        advance();
    }

    if (currentChar == '\0') {
        return {TokenType::INVALID, "Unterminated string", line, startColumn};
    }

    advance(); // skip closing "

    return {TokenType::STRING, lexeme, line, startColumn};
}

// --------------------------------------------------
// Read operators and delimiters
// --------------------------------------------------
Token Lexer::readOperatorOrDelimiter() {
    int startColumn = column;
    char ch = currentChar;

    switch (ch) {
        case '+': advance(); return {TokenType::PLUS, "+", line, startColumn};
        case '-': advance(); return {TokenType::MINUS, "-", line, startColumn};
        case '*': advance(); return {TokenType::MUL, "*", line, startColumn};
        case '/': advance(); return {TokenType::DIV, "/", line, startColumn};

        case '=':
            if (peek() == '=') {
                advance(); advance();
                return {TokenType::EQ, "==", line, startColumn};
            }
            advance();
            return {TokenType::ASSIGN, "=", line, startColumn};

        case '<': advance(); return {TokenType::LT, "<", line, startColumn};
        case '>': advance(); return {TokenType::GT, ">", line, startColumn};

        case '(':
            advance(); return {TokenType::LPAREN, "(", line, startColumn};
        case ')':
            advance(); return {TokenType::RPAREN, ")", line, startColumn};
        case '{':
            advance(); return {TokenType::LBRACE, "{", line, startColumn};
        case '}':
            advance(); return {TokenType::RBRACE, "}", line, startColumn};
        case ';':
            advance(); return {TokenType::SEMICOLON, ";", line, startColumn};

        default:
            advance();
            return {TokenType::INVALID, std::string(1, ch), line, startColumn};
    }
}

// --------------------------------------------------
// Main lexer entry point
// --------------------------------------------------
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

        if (currentChar == '"') {
            return readString();
        }

        return readOperatorOrDelimiter();
    }

    return {TokenType::END_OF_FILE, "", line, column};
}
