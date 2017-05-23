#include "dfxam.h"

using namespace dfxam::ast;

Assignment::Assignment(Function* f, Expression* expr)
    : f(f), expr(expr) {}

Function* Assignment::getFunction() const {
    return f;
}

Expression* Assignment::getExpression() const {
    return expr;
}

Expression* Assignment::derivative(Function* respect) {
    return nullptr;
}

Expression* Assignment::simplify(repl::ExecutionEngine* eng) {
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
