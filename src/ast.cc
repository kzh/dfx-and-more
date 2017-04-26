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
//    std::cout << "Simplifying: " << this << std::endl;

    return clone();
}

std::string Constant::toString() const {
    std::stringstream s;
    s << value;

    return s.str();
}

Expression* Constant::clone() {
    return new Constant(getValue());
}

bool Constant::isConstant() const {
    return true;
}

E::E() 
    : Constant(exp(1)) {}

std::string E::toString() const {
    return "e";
}

Expression* E::clone() {
    return new E();
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
//    std::cout << "Simplifying: " << this << std::endl;

    return clone();
}

std::string Variable::toString() const {
    std::stringstream s;
    s << variable;

    return s.str();
}

Expression* Variable::clone() {
    return new Variable(getVariable());
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

Expression* Differentiation::clone() {
    return new Differentiation(expr->clone(), 
                               new Variable(respect->getVariable()));
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
    auto ln = new Log(new E(), getLeft()->clone());
    auto product = new Product(getRight()->clone(), ln);
    auto divide = new Quotient(new Constant(1), product);

    return new Product(divide, getRight()->derivative(respect));
}

//TODO
Expression* Log::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

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

Power::Power(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) ^ g(x))' = (e ^ ln(f(x) ^ g(x))) * (g(x)ln(f(x)))' 
Expression* Power::derivative(Variable* respect) {
    auto left = new Power(new E(), new Log(new E(), clone()));
    auto right = new Product(getRight()->clone(), new Log(new E(), getLeft()->clone()));

    return new Product(left->clone(), right->derivative(respect));
}

//TODO
Expression* Power::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    return new Power(getLeft()->simplify(), getRight()->simplify());
}

std::string Power::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") ^ (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Power::clone() {
    return new Power(getLeft()->clone(), getRight()->clone());
}

Sum::Sum(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) + g(x))'  = f'(x) + g'(x)
Expression* Sum::derivative(Variable* respect) {
    return new Sum(getLeft()->derivative(respect), getRight()->derivative(respect));
}

Expression* Sum::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    Expression* left = getLeft()->simplify();
    Expression* right = getRight()->simplify();

    if (left->isConstant()) {
        Constant* lConstant = static_cast<Constant*>(left);

        if (lConstant->getValue() == 0) {
            return right; 
        }
    }

    if (right->isConstant()) {
        Constant* rConstant = static_cast<Constant*>(right);

        if (rConstant->getValue() == 0) {
            return left;
        }
    }


    return new Sum(left, right);
}

std::string Sum::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") + (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Sum::clone() {
    return new Sum(getLeft()->clone(), getRight()->clone());
}

Difference::Difference(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) - g(x))' = f'(x) - g'(x)
Expression* Difference::derivative(Variable* respect) {
    return new Difference(getLeft()->derivative(respect), getRight()->derivative(respect));
}

//TODO
Expression* Difference::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    return new Difference(getLeft()->simplify(), getRight()->simplify());
}

std::string Difference::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") - (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Difference::clone() {
    return new Difference(getLeft()->clone(), getRight()->clone());
}

Product::Product(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) * g(x))' = f'(x)g(x) + f(x)g'(x)
Expression* Product::derivative(Variable* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight()->clone());
    auto right = new Product(getLeft()->clone(), getRight()->derivative(respect));

    return new Sum(left, right);
}

//TODO
Expression* Product::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    Expression* left = getLeft()->simplify();
    Expression* right = getRight()->simplify();

    Constant* lConstant = nullptr;
    Constant* rConstant = nullptr;
    if (left->isConstant()) {
        lConstant = static_cast<Constant*>(left);

        if (lConstant->getValue() == 0) {
            return new Constant(0); 
        }
    }

    if (right->isConstant()) {
        rConstant = static_cast<Constant*>(right);

        if (rConstant->getValue() == 0) {
            return new Constant(0);
        }
    }

    if (left->isConstant() && lConstant->getValue() == 1) {
        return right;
    } else if (right->isConstant() && rConstant->getValue() == 1) {
        return left;
    }

    return new Product(left, right);;
}

std::string Product::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") * (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Product::clone() {
    return new Product(getLeft()->clone(), getRight()->clone());
}

Quotient::Quotient(Expression* left, Expression* right)
    : BinaryOperator(left, right) {}

// (f(x) / g(x))' = (f'(x)g(x) - f(x)g'(x)) / (g(x) ^ 2)
Expression* Quotient::derivative(Variable* respect) {
    auto left = new Product(getLeft()->derivative(respect), getRight()->clone());
    auto right = new Product(getLeft()->clone(), getRight()->derivative(respect));

    auto diff = new Difference(left, right);
    auto denom = new Power(getRight()->clone(), new Constant(2));

    return new Quotient(diff, denom);
}

//TODO
Expression* Quotient::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    return new Quotient(getLeft()->simplify(), getRight()->simplify());
}

std::string Quotient::toString() const {
    std::stringstream s;
    s << "(" << getLeft()->toString() << ") / (" << getRight()->toString() << ")";

    return s.str();
}

Expression* Quotient::clone() {
    return new Quotient(getLeft()->clone(), getRight()->clone());
}

std::ostream& operator <<(std::ostream& out, const dfxam::ast::Expression* expr) {
    return out << expr->toString();
}
