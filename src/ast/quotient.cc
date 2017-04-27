#include "dfxam.h"

using namespace dfxam::ast;

Quotient::Quotient(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) / g(x))' = (f'(x)g(x) - f(x)g'(x)) / (g(x) ^ 2)
Expression* Quotient::derivative(Variable* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight()->clone());
    auto right = new Product(getLeft()->clone(), getRight()->derivative(respect));

    auto diff = new Difference(left, right);
    auto denom = new Power(getRight()->clone(), new Constant(2));

    return new Quotient(diff, denom);
}

//TODO
Expression* Quotient::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    return new Quotient(getLeft()->simplify(), getRight()->simplify());
}

std::string Quotient::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") / (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Quotient::clone() {
    return new Quotient(getLeft()->clone(), getRight()->clone());
}
