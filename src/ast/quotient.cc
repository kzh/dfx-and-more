#include "dfxam.h"

using namespace dfxam::ast;

Quotient::Quotient(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) / g(x))' = (f'(x)g(x) - f(x)g'(x)) / (g(x) ^ 2)
Expression* Quotient::derivative(repl::ExecutionEngine* eng, Function* respect) {
    auto left = new Product(getLeft()->derivative(eng, respect), getRight()->clone());
    auto right = new Product(getLeft()->clone(), getRight()->derivative(eng, respect));

    auto diff = new Difference(left, right);
    auto denom = new Power(getRight()->clone(), new Constant(2));

    return new Quotient(diff, denom);
}

Expression* Quotient::substitute(repl::ExecutionEngine* eng) {
    return new Quotient(getLeft()->substitute(eng), getRight()->substitute(eng));
}

//TODO
Expression* Quotient::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;
    auto left = getLeft()->simplify(eng);
    auto right = getRight()->simplify(eng);

    if (left->isConstant() && right->isConstant()) {
        double lVal = static_cast<Constant*>(left)->getValue();
        double rVal = static_cast<Constant*>(right)->getValue();

        delete left;
        delete right;

        return new Constant(lVal / rVal);
    } 

    if (left->equals(eng, right)) {
        delete left;
        delete right;

        return new Constant(1);
    }

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

bool Quotient::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Quotient* q = nullptr;
    if (!(q = dynamic_cast<Quotient*>(expr))) {
        return false;
    }

    return getLeft()->equals(eng, q->getLeft()) && getRight()->equals(eng, q->getRight());
}

Expression* Quotient::clone() {
    return new Quotient(getLeft()->clone(), getRight()->clone());
}
