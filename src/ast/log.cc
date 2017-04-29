#include "dfxam.h"

using namespace dfxam::ast;

Log::Log(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

//log_a(f(x))' = 1/(f(x)ln(a)) * f'(x)
Expression* Log::derivative(Variable* respect) {
    auto ln = new Log(new E(), getLeft()->clone());
    auto product = new Product(getRight()->clone(), ln);
    auto divide = new Quotient(new Constant(1), product);

    return new Product(divide, getRight()->derivative(respect));
}

Expression* Log::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    auto left = getLeft()->simplify();
    auto right = getRight()->simplify();

    // log_x(x) =  1
    if (left->equals(right)) {
        delete left;
        delete right;
        return new Constant(1);
    }

    return new Log(left, right);
}

std::string Log::toString() const {
    std::stringstream s;

    s << "log_(" << getLeft()->toString() << ")(" << getRight()->toString() << ")"; 

    return s.str();
}

bool Log::equals(Expression* expr) {
    Log* l = nullptr;
    if (!(l = dynamic_cast<Log*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify();
    auto thisR = getRight()->simplify();
    auto exprL = l->getLeft()->simplify();
    auto exprR = l->getRight()->simplify();

    bool equality = thisL->equals(exprL) && thisR->equals(exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Log::clone() {
    return new Log(getLeft()->clone(), getRight()->clone());
}
