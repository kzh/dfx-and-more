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
    if (!match(repl::TokenType::IDENTIFIER, "", 0) ||
        !match(repl::TokenType::SEPARATOR, "(", 1)) {
        return nullptr;
    }

    ast::Function* name = new ast::Function(peek().getContents());

    int rollback = index;
    consume();

    ast::Expression* func = parseInvocation(name);
    if (!func || func == name || !match(repl::TokenType::OPERATOR, "=", 0)) {
        index = rollback;

        if (func) {
            delete func;
        }

        return nullptr;
    }
    consume();

    ast::Expression* expr = parseExpression();
    return new ast::Assignment(static_cast<ast::Invocation*>(func), expr);
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
    if (match(TokenType::OPERATOR, "-", 0)) {
        consume();

        ast::Constant* neg = new ast::Constant(-1);
        return new ast::Product(neg, parseAtom());
    }

    ast::Expression* expr = nullptr;
    if (ast::Expression* constant = parseConstant()) {
        expr = constant;
    } else if (match(TokenType::IDENTIFIER, "", 0)) {
        expr = new ast::Function(peek().getContents());
        consume();
    } else if (match(TokenType::SEPARATOR, "(", 0)) {
        consume();
        expr = parseExpression();

        if (match(TokenType::SEPARATOR, ")", 0)) {
            consume();
        }
    }

    while (match(TokenType::OPERATOR, "'", 0)) {
        expr = new ast::Differentiation(expr);
        consume();
    }

    return parseInvocation(expr);
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

ast::Expression* repl::Parser::parseInvocation(ast::Expression* expr) {
    if (!expr || !match(repl::TokenType::SEPARATOR, "(", 0)) {
        return expr;
    }
    consume();

    std::vector<ast::Expression*> args;
    while (!match(repl::TokenType::SEPARATOR, ")", 0)) {
        args.push_back(parseExpression());

        if (match(repl::TokenType::SEPARATOR, ",", 0)) {
            consume();
        }
    }
    consume();

    ast::Invocation* f  = new ast::Invocation(expr, args);
    return f;
}
