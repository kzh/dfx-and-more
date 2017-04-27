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

    return new Log(getLeft()->clone(), getRight()->clone());
}

std::string Log::toString() const {
    std::stringstream s;

    s << "log_(" << getLeft()->toString() << ")(" << getRight()->toString() << ")"; 

    return s.str();
}

Expression* Log::clone() {
    return new Log(getLeft()->clone(), getRight()->clone());
}
