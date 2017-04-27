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

    if (left->isConstant()) {
        Constant* lConstant = static_cast<Constant*>(left);

        if (lConstant->getValue() == 0) {
            return right; 
        }
    }

    if (right->isConstant()) {
        Constant* rConstant = static_cast<Constant*>(right);

        if (rConstant->getValue() == 0) {
            return left;
        }
    }


    return new Sum(left, right);
}

std::string Sum::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") + (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Sum::clone() {
    return new Sum(getLeft()->clone(), getRight()->clone());
}
