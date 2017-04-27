#include "dfxam.h"

using namespace dfxam::ast;

Differentiation::Differentiation(Expression* expr, Variable* respect)
    : expr(expr), respect(respect) {}

Expression* Differentiation::derivative(Variable* respect) {
    return new Differentiation(this, respect);
}

Expression* Differentiation::simplify() {
    return expr->derivative(respect);
}

std::string Differentiation::toString() const {
    std::stringstream s;
    s << "d/d" << respect->toString() << "(" << expr->toString() << ")";

    return s.str();
}

Expression* Differentiation::clone() {
    return new Differentiation(expr->clone(), 
                               new Variable(respect->getVariable()));
}

Differentiation::~Differentiation() {
    delete expr;
    delete respect;
}
