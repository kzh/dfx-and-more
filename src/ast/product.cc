#include "dfxam.h"

using namespace dfxam::ast;

Product::Product(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) * g(x))' = f'(x)g(x) + f(x)g'(x)
Expression* Product::derivative(Function* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight()->clone());
    auto right = new Product(getLeft()->clone(), getRight()->derivative(respect));

    return new Sum(left, right);
}

//TODO
Expression* Product::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;

    Expression* left = getLeft()->simplify(eng);
    Expression* right = getRight()->simplify(eng);

    // multiplicative property of zero
    if (Constant::isConstantValue(left, 0) ||
        Constant::isConstantValue(right, 0)) {
            delete left;
            delete right;
            return new Constant(0); 
    }

    // multiplicative identity property
    if (Constant::isConstantValue(left, 1)) {
        delete left;
        return right;
    } else if (Constant::isConstantValue(right, 1)) {
        delete right;
        return left;
    }

    return new Product(left, right);;
}

std::string Product::toString() const {
    std::stringstream s;

    s << "(" << getLeft()->toString() << ") * (" << getRight()->toString() << ")";

    return s.str();
}

bool Product::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Product* p = nullptr;
    if (!(p = dynamic_cast<Product*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify(eng);
    auto thisR = getRight()->simplify(eng);
    auto exprL = p->getLeft()->simplify(eng);
    auto exprR = p->getRight()->simplify(eng);

    bool equality = thisL->equals(eng, exprL) && thisR->equals(eng, exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Product::clone() {
    return new Product(getLeft()->clone(), getRight()->clone());
}
