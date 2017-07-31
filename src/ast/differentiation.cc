#include "dfxam.h"

using namespace dfxam::ast;

Differentiation::Differentiation(Expression* expr)
    : expr(expr), respect(nullptr) {}

Differentiation::Differentiation(Expression* expr, Function* respect)
    : expr(expr), respect(respect) {}

Expression* Differentiation::getExpression() const {
    return expr;
}

Function* Differentiation::getRespect() const {
    return respect;
}

Expression* Differentiation::derivative(repl::ExecutionEngine* eng, Function* respect) {
    Expression* simpl = simplify(eng);
    Expression* derivative = simpl->derivative(eng, respect);
    Expression* res = derivative->simplify(eng);

    delete simpl;
    delete derivative;

    return res;
}

Expression* Differentiation::substitute(repl::ExecutionEngine* eng) {
    Expression* subExpr = expr->substitute(eng);

    if (!respect) {
        return new Differentiation(subExpr);
    }

    Expression* subRespect = respect->substitute(eng);
    Function* resp;

    if (!(resp = dynamic_cast<Function*>(subRespect))) {
        delete subExpr;
        delete subRespect;

        return nullptr;
    }

    return new Differentiation(subExpr, resp);
}

Expression* Differentiation::simplify(repl::ExecutionEngine* eng) {
    if (!respect) {
        respect = expr->getVar(eng);
    }

    Expression* simpl = expr->simplify(eng);
    Expression* derivative = simpl->derivative(eng, respect);
    Expression* res = derivative->simplify(eng);

    delete simpl;
    delete derivative;

    return res;
}

Function* Differentiation::getVar(repl::ExecutionEngine* eng) {
    if (respect) {
        return respect;
    }

    return expr->getVar(eng);
}

std::string Differentiation::toString() const {
    std::stringstream s;
    if (respect) {
        s << "d/d" << respect << "(" << expr << ")";
    } else {
        s << "(" << expr << ")'";
    }

    return s.str();
}

Expression* Differentiation::clone() {
    Function* resp = respect ? new Function(respect->getName()) : nullptr;
    return new Differentiation(expr->clone(), resp);
}

bool Differentiation::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Differentiation* d = nullptr;
    if (!(d = dynamic_cast<Differentiation*>(expr))) {
        return false;
    }

    auto thisExpr = getExpression()->simplify(eng);
    auto otherExpr = d->getExpression()->simplify(eng); 

    bool equality = thisExpr->equals(eng, otherExpr) && getRespect()->getName() == d->getRespect()->getName();

    delete thisExpr;
    delete otherExpr;

    return equality;
}

Differentiation::~Differentiation() {
    delete expr;

    if (respect) {
        delete respect;
    }
}
