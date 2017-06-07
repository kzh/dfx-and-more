#include "dfxam.h"

using namespace dfxam::repl;

Token::Token(TokenType type, std::string contents)
    : type(type), contents(contents) {}

bool Token::match(TokenType t, std::string c) {
    return type == t && (c == contents || c == ""); 
}

std::string Token::getContents() const {
    return contents;
}

TokenType Token::getType() const {
    return type;
}

Lexer::Lexer(std::string s)
    : s(s), index(0) {}

char& Lexer::peek() {
    return s[index];
}

void Lexer::consume() {
    index++;
}

std::vector<Token> Lexer::lex() {
    while (index < s.length()) {
        char& c = peek();
        if (isLetter(c)) {
            lexIdentifier();
        } else if (isNumber(c) || c == '.') {
            lexNumber();
        } else if (isSeparator(c)) {
            lexSeparator();
        } else if (isOperator(c)) {
            lexOperator();
        } else if (c == ' ') {
            consume();
        } else {
            throw "Unexpected character: " + std::string(1, c);
        }
    }

    return tokens;
}

void Lexer::lexNumber() {
    char& c = peek();
    if (!isNumber(c) && c != '.') {
        return;
    }

    std::stringstream s;
    while (isNumber(c = peek()) || c == '.') {
        s << c;
        consume();
    }

    Token t(TokenType::NUMBER, s.str());
    tokens.push_back(t);
}

void Lexer::lexIdentifier() {
    char& c = peek();
    if (!isLetter(c)) {
        return;
    }

    std::stringstream s;
    while (isLetter(c = peek())) {
        s << c;
        consume();
    }

    Token t(TokenType::IDENTIFIER, s.str());
    tokens.push_back(t);
}

void Lexer::lexOperator() {
    char& c = peek();
    if (!isOperator(c)) {
        return;
    }

    consume();

    Token t(TokenType::OPERATOR, std::string(1, c));
    tokens.push_back(t);
}

void Lexer::lexSeparator() {
    char& c = peek();
    if (!isSeparator(c)) {
        return;
    }

    consume();

    Token t(TokenType::SEPARATOR, std::string(1, c));
    tokens.push_back(t);
}

bool Lexer::isOperator(char c) {
    std::string operators = "+-*/=^'";
    return operators.find(c) != std::string::npos;
}

bool Lexer::isSeparator(char c) {
    std::string separators = "(),_";
    return separators.find(c) != std::string::npos;
}

bool Lexer::isLetter(char c) {
    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
}

bool Lexer::isNumber(char c) {
    return c >= 48 && c <= 57;
}
