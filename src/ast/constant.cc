#include <math.h>

#include "dfxam.h"

using namespace dfxam::ast;

Constant::Constant(double val)
    : value(val) {}

double Constant::getValue() const {
    return value;
}

Expression* Constant::derivative(repl::ExecutionEngine* eng, Function* respect) {
    return new Constant(0);
}

Expression* Constant::substitute(repl::ExecutionEngine* eng) {
    return clone();
}

Expression* Constant::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;

    return clone();
}

std::string Constant::toString() const {
    std::stringstream s;
    s << value;

    return s.str();
}

bool Constant::equals(repl::ExecutionEngine* eng, Expression* expr) {
    if (!expr->isConstant()) {
        return false;
    }

    Constant* c = static_cast<Constant*>(expr);
    return c->getValue() == getValue();
}

Expression* Constant::clone() {
    return new Constant(getValue());
}

bool Constant::isConstant() const {
    return true;
}

bool Constant::isConstantValue(Expression* expr, int val) {
    if (!expr->isConstant()) {
        return false;
    }

    Constant* c = static_cast<Constant*>(expr);
    return c->getValue() == val;
}

E::E() 
    : Constant(exp(1)) {}

std::string E::toString() const {
    return "e";
}

Expression* E::clone() {
    return new E();
}
