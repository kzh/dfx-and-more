#include "dfxam.h"

using namespace dfxam::ast;

Difference::Difference(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) - g(x))' = f'(x) - g'(x)
Expression* Difference::derivative(Variable* respect) {
    return new Difference(getLeft()->derivative(respect), getRight()->derivative(respect));
}

//TODO
Expression* Difference::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    return new Difference(getLeft()->simplify(), getRight()->simplify());
}

std::string Difference::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") - (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Difference::clone() {
    return new Difference(getLeft()->clone(), getRight()->clone());
}
