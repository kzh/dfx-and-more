#include "dfxam.h"

using namespace dfxam::ast;

Sum::Sum(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) + g(x))'  = f'(x) + g'(x)
Expression* Sum::derivative(repl::ExecutionEngine* eng, Function* respect) {
    return new Sum(getLeft()->derivative(eng, respect), getRight()->derivative(eng, respect));
}

Expression* Sum::substitute(repl::ExecutionEngine* eng) {
    return new Sum(getLeft()->substitute(eng), getRight()->substitute(eng));
}

Expression* Sum::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;

    Expression* left = getLeft()->simplify(eng);
    Expression* right = getRight()->simplify(eng);

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

bool Sum::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Sum* s = nullptr;
    if (!(s = dynamic_cast<Sum*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify(eng);
    auto thisR = getRight()->simplify(eng);
    auto exprL = s->getLeft()->simplify(eng);
    auto exprR = s->getRight()->simplify(eng);

    bool equality = thisL->equals(eng, exprL) && thisR->equals(eng, exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Sum::clone() {
    return new Sum(getLeft()->clone(), getRight()->clone());
}
