#include "dfxam.h"

using namespace dfxam::ast;

Product::Product(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) * g(x))' = f'(x)g(x) + f(x)g'(x)
Expression* Product::derivative(Variable* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight()->clone());
    auto right = new Product(getLeft()->clone(), getRight()->derivative(respect));

    return new Sum(left, right);
}

//TODO
Expression* Product::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    Expression* left = getLeft()->simplify();
    Expression* right = getRight()->simplify();

    Constant* lConstant = nullptr;
    Constant* rConstant = nullptr;

    // multiplicative property of zero
    if (left->isConstant()) {
        lConstant = static_cast<Constant*>(left);

        if (lConstant->getValue() == 0) {
            return new Constant(0); 
        }
    }

    if (right->isConstant()) {
        rConstant = static_cast<Constant*>(right);

        if (rConstant->getValue() == 0) {
            return new Constant(0);
        }
    }

    // multiplicative identity property
    if (left->isConstant() && lConstant->getValue() == 1) {
        return right;
    } else if (right->isConstant() && rConstant->getValue() == 1) {
        return left;
    }

    return new Product(left, right);;
}

std::string Product::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") * (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Product::clone() {
    return new Product(getLeft()->clone(), getRight()->clone());
}
