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

    if (left->isConstant() && right->isConstant()) {
        double lVal = static_cast<Constant*>(left)->getValue();
        double rVal = static_cast<Constant*>(right)->getValue();

        delete left;
        delete right;

        return new Constant(lVal + rVal);
    } 

    /* additive identity property */
    if (Constant::isConstantValue(left, 0)) {
        delete left;
        return right;
    }

    if (Constant::isConstantValue(right, 0)) {
        delete right;
        return left;
    }

    Product* lProd;
    Product* rProd;
    if ((lProd = dynamic_cast<Product*>(left)) &&
        (rProd = dynamic_cast<Product*>(right))) {
        Expression* lSum;
        Expression* rSum;
        Expression* mult;

        if (lProd->getRight()->equals(eng, rProd->getRight())) {
            mult = lProd->getRight();
            lSum = lProd->getLeft();
            rSum = rProd->getLeft();
        } else if (lProd->getLeft()->equals(eng, rProd->getLeft())) {
            mult = lProd->getLeft();
            lSum = lProd->getRight();
            rSum = rProd->getRight();
        } else if (lProd->getLeft()->equals(eng, rProd->getRight())) {
            mult = lProd->getLeft();
            lSum = lProd->getRight();
            rSum = rProd->getLeft();
        } else if (lProd->getRight()->equals(eng, rProd->getLeft())) {
            mult = lProd->getRight();
            lSum = lProd->getLeft();
            rSum = rProd->getRight();
        }

        Sum* s = new Sum(lSum->clone(), rSum->clone());
        Product* p = new Product(s, mult->clone());
        Expression* simpl = p->simplify(eng);

        delete left;
        delete right;
        delete p;

        return simpl;
    }

    if (left->equals(eng, right)) {
        delete right;

        Product* product = new Product(new Constant(2), left);
        Expression* simpl = product->simplify(eng);

        return simpl;
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

    return getLeft()->equals(eng, s->getLeft()) && getRight()->equals(eng, s->getRight());
}

Expression* Sum::clone() {
    return new Sum(getLeft()->clone(), getRight()->clone());
}
