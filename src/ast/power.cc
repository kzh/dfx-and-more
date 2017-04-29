#include "dfxam.h"

using namespace dfxam::ast;

Power::Power(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) ^ g(x))' = (e ^ ln(f(x) ^ g(x))) * (g(x)ln(f(x)))' 
Expression* Power::derivative(Variable* respect) {
    auto left = new Power(new E(), new Log(new E(), clone()));
    auto right = new Product(getRight()->clone(), new Log(new E(), getLeft()->clone()));

    return new Product(right->derivative(respect), left->clone());
}

//TODO
Expression* Power::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;
    auto left = getLeft()->simplify();
    auto right = getRight()->simplify();

    if (Log* l = dynamic_cast<Log*>(right)) {
        if (left->equals(l->getLeft())) {
            return l->getRight()->clone();
        }
    }

    return new Power(getLeft()->simplify(), getRight()->simplify());
}

std::string Power::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") ^ (" << getRight()->toString() << ")";

    return s.str();
}

bool Power::equals(Expression* expr) {
    Power* p = nullptr;
    if (!(p = dynamic_cast<Power*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify();
    auto thisR = getRight()->simplify();
    auto exprL = p->getLeft()->simplify();
    auto exprR = p->getRight()->simplify();

    bool equality = thisL->equals(exprL) && thisR->equals(exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Power::clone() {
    return new Power(getLeft()->clone(), getRight()->clone());
}
