#include <stdlib.h>

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

repl::Token& repl::Parser::peek() {
    return tokens[index];
}

void repl::Parser::consume() {
    index++;
}

ast::Expression* repl::Parser::parse() {
    if (ast::Expression* assignment = parseAssignment()) {
        return assignment;
    } else if (ast::Expression* expr = parseExpression()) {
        return expr;
    }

    return nullptr;
}

ast::Expression* repl::Parser::parseAssignment() {
    int rollback = index;

    ast::Expression* func = parseInvocation();
    if (!func || !match(repl::TokenType::OPERATOR, "=", 0)) {
        index = rollback;
        return nullptr;
    }
    consume();

    ast::Expression* expr = parseExpression();
    return new ast::Assignment(static_cast<ast::Function*>(func), expr);
}

ast::Expression* repl::Parser::parseExpression(int minPrecedence) {
    ast::Expression* lhs = parseAtom();

    while (match(TokenType::OPERATOR, "", 0)) {
        std::string op = peek().getContents();
        OperatorInfo info = OP_INFO[op];

        int prec = std::get<1>(info);
        if (prec < minPrecedence) {
            break;
        }
        consume();

        if (std::get<0>(info) == Associativity::LEFT) {
            prec++;
        }

        ast::Expression* rhs = parseExpression(prec);
        lhs = parseBinop(op, lhs, rhs);
    }

    return lhs;
}

ast::Expression* repl::Parser::parseAtom() {
    if (ast::Expression* constant = parseConstant()) {
        return constant;
    } else if (ast::Expression* invoc = parseInvocation()) {
        return invoc;
    } else if (match(TokenType::SEPARATOR, "(", 0)) {
        consume();
        ast::Expression* expr = parseExpression();

        if (match(TokenType::SEPARATOR, ")", 0)) {
            consume();
            return expr;
        }
    }

    return nullptr;
}

ast::Expression* repl::Parser::parseBinop(std::string op, ast::Expression* lhs, ast::Expression* rhs) {
    if (op == "+") {
        return new ast::Sum(lhs, rhs);
    } else if (op == "*") {
        return new ast::Product(lhs, rhs);
    } else if (op == "-") {
        return new ast::Difference(lhs, rhs);
    } else if (op == "/") {
        return new ast::Quotient(lhs, rhs);
    } else if (op == "^") {
        return new ast::Power(lhs, rhs);
    }

    return nullptr;
}

ast::Expression* repl::Parser::parseConstant() {
    if (match(repl::TokenType::NUMBER, "", 0)) {
        double d = atof(peek().getContents().c_str());
        consume();

        return new ast::Constant(d);
    }

    if (match(repl::TokenType::IDENTIFIER, "e", 0)) {
        consume();

        return new ast::E();
    }

    return nullptr;
}

ast::Expression* repl::Parser::parseInvocation() {
    if (!match(repl::TokenType::IDENTIFIER, "", 0)) {
        return nullptr;
    }

    Token& name = peek();
    consume();

    std::vector<ast::Expression*> args;
    if (match(repl::TokenType::SEPARATOR, "(", 0)) {
        consume();

        while (!match(repl::TokenType::SEPARATOR, ")", 0)) {
            args.push_back(parseExpression());

            if (match(repl::TokenType::SEPARATOR, ",", 0)) {
                consume();
            }
        }

        consume();
    }

    return new ast::Function(name.getContents(), args); 
}
