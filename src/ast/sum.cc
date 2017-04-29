#include "dfxam.h"

using namespace dfxam::ast;

Sum::Sum(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) + g(x))'  = f'(x) + g'(x)
Expression* Sum::derivative(Variable* respect) {
    return new Sum(getLeft()->derivative(respect), getRight()->derivative(respect));
}

Expression* Sum::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    Expression* left = getLeft()->simplify();
    Expression* right = getRight()->simplify();

    /* additive identity property */

    if (Constant::isConstantValue(left, 0)) {
        delete left;
        return right; 
    }

    if (Constant::isConstantValue(right, 0)) {
        delete right;
        return left;
    }

    return new Sum(left, right);
}

std::string Sum::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") + (" << getRight()->toString() << ")";

    return s.str();
}

bool Sum::equals(Expression* expr) {
    Sum* s = nullptr;
    if (!(s = dynamic_cast<Sum*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify();
    auto thisR = getRight()->simplify();
    auto exprL = s->getLeft()->simplify();
    auto exprR = s->getRight()->simplify();

    bool equality = thisL->equals(exprL) && thisR->equals(exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Sum::clone() {
    return new Sum(getLeft()->clone(), getRight()->clone());
}
