#include "dfxam.h"

using namespace dfxam::ast;

Power::Power(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) ^ g(x))' = (e ^ ln(f(x) ^ g(x))) * (g(x)ln(f(x)))' 
Expression* Power::derivative(repl::ExecutionEngine* eng, Function* respect) {
    auto left = new Power(new E(), new Log(new E(), clone()));
    auto right = new Product(getRight()->clone(), new Log(new E(), getLeft()->clone()));

    return new Product(right->derivative(eng, respect), left->clone());
}

Expression* Power::substitute(repl::ExecutionEngine* eng) {
    return new Power(getLeft()->substitute(eng), getRight()->substitute(eng));
}

//TODO
Expression* Power::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;
    auto left = getLeft()->simplify(eng);
    auto right = getRight()->simplify(eng);

    // a ^ log_a(x) = x
    if (Log* l = dynamic_cast<Log*>(right)) {
        if (left->equals(eng, l->getLeft())) {
            auto r = l->getRight()->clone();

            delete left;
            delete right;

            return r; 
        }
    }

    return new Power(left, right);
}

std::string Power::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") ^ (" << getRight()->toString() << ")";

    return s.str();
}

bool Power::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Power* p = nullptr;
    if (!(p = dynamic_cast<Power*>(expr))) {
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

Expression* Power::clone() {
    return new Power(getLeft()->clone(), getRight()->clone());
}
