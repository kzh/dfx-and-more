#include "dfxam.h"

using namespace dfxam::ast;

Difference::Difference(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) - g(x))' = f'(x) - g'(x)
Expression* Difference::derivative(repl::ExecutionEngine* eng, Function* respect) {
    return new Difference(getLeft()->derivative(eng, respect), getRight()->derivative(eng, respect));
}

Expression* Difference::substitute(repl::ExecutionEngine* eng) {
    return new Difference(getLeft()->substitute(eng), getRight()->substitute(eng));
}

Expression* Difference::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;
    auto left = getLeft()->simplify(eng);
    auto right = getRight()->simplify(eng);

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

bool Difference::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Difference* d = nullptr;
    if (!(d = dynamic_cast<Difference*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify(eng);
    auto thisR = getRight()->simplify(eng);
    auto exprL = d->getLeft()->simplify(eng);
    auto exprR = d->getRight()->simplify(eng);

    bool equality = thisL->equals(eng, exprL) && thisR->equals(eng, exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Difference::clone() {
    return new Difference(getLeft()->clone(), getRight()->clone());
}
