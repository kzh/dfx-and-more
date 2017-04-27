#include "dfxam.h"

using namespace dfxam::ast;

Power::Power(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) ^ g(x))' = (e ^ ln(f(x) ^ g(x))) * (g(x)ln(f(x)))' 
Expression* Power::derivative(Variable* respect) {
    auto left = new Power(new E(), new Log(new E(), clone()));
    auto right = new Product(getRight()->clone(), new Log(new E(), getLeft()->clone()));

    return new Product(left->clone(), right->derivative(respect));
}

//TODO
Expression* Power::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;
    auto left = getLeft()->simplify();
    auto right = getRight()->simplify();

    if (left->isConstant()) {

    }

    return new Power(getLeft()->simplify(), getRight()->simplify());
}

std::string Power::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") ^ (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Power::clone() {
    return new Power(getLeft()->clone(), getRight()->clone());
}
