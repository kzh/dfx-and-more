#include "dfxam.h"

using namespace dfxam::ast;

Difference::Difference(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) - g(x))' = f'(x) - g'(x)
Expression* Difference::derivative(Function* respect) {
    return new Difference(getLeft()->derivative(respect), getRight()->derivative(respect));
}

//TODO
Expression* Difference::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;
    auto left = getLeft()->simplify();
    auto right = getRight()->simplify();

    // 0 - x = -1 * x
    if (Constant::isConstantValue(left, 0)) {
        delete left;
        return new Product(new Constant(-1), right);
    }

    // x - 0 = x
    if (Constant::isConstantValue(right, 0)) {
        delete right;
        return left;
    }

    return new Difference(left, right);
}

std::string Difference::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") - (" << getRight()->toString() << ")";

    return s.str();
}

bool Difference::equals(Expression* expr) {
    Difference* d = nullptr;
    if (!(d = dynamic_cast<Difference*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify();
    auto thisR = getRight()->simplify();
    auto exprL = d->getLeft()->simplify();
    auto exprR = d->getRight()->simplify();

    bool equality = thisL->equals(exprL) && thisR->equals(exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Difference::clone() {
    return new Difference(getLeft()->clone(), getRight()->clone());
}
