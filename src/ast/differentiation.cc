#include "dfxam.h"

using namespace dfxam::ast;

Differentiation::Differentiation(Expression* expr, Function* respect)
    : expr(expr), respect(respect) {}

Expression* Differentiation::getExpression() const {
    return expr;
}

Function* Differentiation::getRespect() const {
    return respect;
}

Expression* Differentiation::derivative(Function* respect) {
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
                               new Function(respect->getName()));
}

bool Differentiation::equals(Expression* expr) {
    Differentiation* d = nullptr;
    if (!(d = dynamic_cast<Differentiation*>(expr))) {
        return false;
    }

    auto thisExpr = getExpression()->simplify();
    auto otherExpr = d->getExpression()->simplify(); 

    bool equality = thisExpr->equals(otherExpr) && getRespect()->getName() == d->getRespect()->getName();

    delete thisExpr;
    delete otherExpr;

    return equality;
}

Differentiation::~Differentiation() {
    delete expr;
    delete respect;
}
