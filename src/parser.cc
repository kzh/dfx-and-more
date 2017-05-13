#include "dfxam.h"

using namespace dfxam;

// TODO

repl::Parser::Parser(std::vector<repl::Token> tokens)
    : tokens(tokens), index(0) {}

bool repl::Parser::match(repl::TokenType t, std::string s, int ahead) {
    if (index + ahead > tokens.size()) {
        return false;
    }

    Token& tok = tokens[index + ahead];
    return tok.getType() == t && (s == "" || tok.getContents() == s);
}

void repl::Parser::consume() {
    index++;
}

ast::Expression* repl::Parser::parse() {

    return nullptr;
}
