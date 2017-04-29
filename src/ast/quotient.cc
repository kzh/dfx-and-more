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
    auto left = getLeft()->simplify();
    auto right = getRight()->simplify();

    if (Constant::isConstantValue(left, 0)) {
        delete left;
        delete right;
        return new Constant(0);
    }

    return new Quotient(left, right);
}

std::string Quotient::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") / (" << getRight()->toString() << ")";

    return s.str();
}

bool Quotient::equals(Expression* expr) {
    Quotient* q = nullptr;
    if (!(q = dynamic_cast<Quotient*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify();
    auto thisR = getRight()->simplify();
    auto exprL = q->getLeft()->simplify();
    auto exprR = q->getRight()->simplify();

    bool equality = thisL->equals(exprL) && thisR->equals(exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Quotient::clone() {
    return new Quotient(getLeft()->clone(), getRight()->clone());
}
