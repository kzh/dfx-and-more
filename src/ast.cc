#include <math.h>

#include "dfxam.h"

using namespace dfxam::ast;

bool Expression::isConstant() const {
    return false;
}

Constant::Constant(double val)
    : value(val) {}

double Constant::getValue() const {
    return value;
}

Expression* Constant::derivative(Variable* respect) {
    return new Constant(0);
}

Expression* Constant::simplify() {
    return this;
}

std::string Constant::toString() const {
    std::stringstream s;
    s << value;

    return s.str();
}

bool Constant::isConstant() const {
    return true;
}

E::E() 
    : Constant(exp(1)) {}

std::string E::toString() const {
    return "e";
}

Variable::Variable(char v)
    : variable(v) {}

char Variable::getVariable() const {
    return variable;
}

Expression* Variable::derivative(Variable* respect) {
    if (respect->getVariable() == variable) {
        return new Constant(1);
    } else {
        return new Differentiation(this, respect);
    }
}

Expression* Variable::simplify() {
    return this;
}

std::string Variable::toString() const {
    std::stringstream s;
    s << variable;

    return s.str();
}

Differentiation::Differentiation(Expression* expr, Variable* respect)
    : expr(expr), respect(respect) {}

Expression* Differentiation::derivative(Variable* respect) {
    return new Differentiation(this, respect);
}

Expression* Differentiation::simplify() {
    return expr->derivative(respect);
}

std::string Differentiation::toString() const {
    std::stringstream s;
    s << "d/d" << respect->toString() << "(" << expr->toString() << ")";

    return s.str();
}

Differentiation::~Differentiation() {
    delete expr;
    delete respect;
}

BinaryOperator::BinaryOperator(Expression* left, Expression* right)
    : left(left), right(right) {}

Expression* BinaryOperator::getLeft() const {
    return left;
}

Expression* BinaryOperator::getRight() const {
    return right;
}

BinaryOperator::~BinaryOperator() {
    delete left;
    delete right;
}

Log::Log(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

//log_a(f(x))' = 1/(f(x)ln(a)) * f'(x)
Expression* Log::derivative(Variable* respect) {
    auto ln = new Log(new E(), getLeft());
    auto product = new Product(getRight(), ln);
    auto divide = new Quotient(new Constant(1), product);

    return new Product(divide, getRight()->derivative(respect));
}

//TODO
Expression* Log::simplify() {
    return this;
}

std::string Log::toString() const {
    std::stringstream s;
    s << "log_(" << getLeft()->toString() << ")(" << getRight()->toString() << ")"; 

    return s.str();
}

Power::Power(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) ^ g(x))' = (e ^ ln(f(x) ^ g(x))) * (g(x)ln(f(x)))' 
Expression* Power::derivative(Variable* respect) {
    auto left = new Power(new E(), new Log(new E(), this));
    auto right = new Product(getRight(), new Log(new E(), getLeft()));

    return new Product(left, right->derivative(respect));
}

//TODO
Expression* Power::simplify() {
    return this;
}

std::string Power::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") ^ (" << getRight()->toString() << ")";

    return s.str();
}

Sum::Sum(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) + g(x))'  = f'(x) + g'(x)
Expression* Sum::derivative(Variable* respect) {
    return new Sum(getLeft()->derivative(respect), getRight()->derivative(respect));
}

//TODO
Expression* Sum::simplify() {
    return this;
}

std::string Sum::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") + (" << getRight()->toString() << ")";

    return s.str();
}

Difference::Difference(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) - g(x))' = f'(x) - g'(x)
Expression* Difference::derivative(Variable* respect) {
    return new Difference(getLeft()->derivative(respect), getRight()->derivative(respect));
}

//TODO
Expression* Difference::simplify() {
    return this;
}

std::string Difference::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") - (" << getRight()->toString() << ")";

    return s.str();
}

Product::Product(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) * g(x))' = f'(x)g(x) + f(x)g'(x)
Expression* Product::derivative(Variable* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight());
    auto right = new Product(getLeft(), getRight()->derivative(respect));

    return new Sum(left, right);
}

//TODO
Expression* Product::simplify() {
    return this;
}

std::string Product::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") * (" << getRight()->toString() << ")";

    return s.str();
}

Quotient::Quotient(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) / g(x))' = (f'(x)g(x) - f(x)g'(x)) / (g(x) ^ 2)
Expression* Quotient::derivative(Variable* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight());
    auto right = new Product(getLeft(), getRight()->derivative(respect));

    auto diff = new Difference(left, right);
    auto denom = new Power(right, new Constant(2));

    return new Quotient(diff, denom);
}

//TODO
Expression* Quotient::simplify() {
    return this;
}

std::string Quotient::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") / (" << getRight()->toString() << ")";

    return s.str();
}


