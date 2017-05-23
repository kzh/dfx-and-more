#include "dfxam.h"

using namespace dfxam::ast;

Log::Log(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

//log_a(f(x))' = 1/(f(x)ln(a)) * f'(x)
Expression* Log::derivative(Function* respect) {
    auto ln = new Log(new E(), getLeft()->clone());
    auto product = new Product(getRight()->clone(), ln);
    auto divide = new Quotient(new Constant(1), product);

    return new Product(divide, getRight()->derivative(respect));
}

Expression* Log::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;

    auto left = getLeft()->simplify(eng);
    auto right = getRight()->simplify(eng);

    // log_x(x) =  1
    if (left->equals(eng, right)) {
        delete left;
        delete right;
        return new Constant(1);
    }

/*
    if (Power* r = dynamic_cast<Power*>(right)) {

    }
*/

    return new Log(left, right);
}

std::string Log::toString() const {
    std::stringstream s;

    s << "log_(" << getLeft()->toString() << ")(" << getRight()->toString() << ")"; 

    return s.str();
}

bool Log::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Log* l = nullptr;
    if (!(l = dynamic_cast<Log*>(expr))) {
        return false;
    }

    auto thisL = getLeft()->simplify(eng);
    auto thisR = getRight()->simplify(eng);
    auto exprL = l->getLeft()->simplify(eng);
    auto exprR = l->getRight()->simplify(eng);

    bool equality = thisL->equals(eng, exprL) && thisR->equals(eng, exprR);

    delete thisL;
    delete thisR;
    delete exprL;
    delete exprR;

    return equality;
}

Expression* Log::clone() {
    return new Log(getLeft()->clone(), getRight()->clone());
}
