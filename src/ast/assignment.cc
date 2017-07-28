#include "dfxam.h"

using namespace dfxam::ast;

Assignment::Assignment(Invocation* f, Expression* expr)
    : f(f), expr(expr) {}

Invocation* Assignment::getDeclaration() const {
    return f;
}

Expression* Assignment::getExpression() const {
    return expr;
}

Expression* Assignment::derivative(repl::ExecutionEngine* eng, Function* respect) {
    return nullptr;
}

Expression* Assignment::substitute(repl::ExecutionEngine* eng) {
    return nullptr;
}

Expression* Assignment::simplify(repl::ExecutionEngine* eng) {
    return nullptr;
}

Function* Assignment::getVar() {
    return nullptr;
}

std::string Assignment::toString() const {
    return "";
}

bool Assignment::equals(repl::ExecutionEngine* eng, Expression* expr) {
    return false;
}

Expression* Assignment::clone() {
    return nullptr;
}

Assignment::~Assignment() {
    delete f;
    delete expr;
}
