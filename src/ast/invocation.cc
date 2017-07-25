#include "dfxam.h"

using namespace dfxam::ast;

Invocation::Invocation(Expression* expr, std::vector<Expression*> arguments)
    : expr(expr), arguments(arguments) {}

Expression* Invocation::getExpression() const {
    return expr;
}

std::vector<Expression*>& Invocation::getArguments() {
    return arguments;
}

Expression* Invocation::derivative(repl::ExecutionEngine* eng, Function* respect) {
    return new Invocation(expr->derivative(eng, respect), arguments);
}

Expression* Invocation::substitute(repl::ExecutionEngine* eng) {
    std::vector<Expression*> subArgs;
    for (int i = 0; i < arguments.size(); i++) {
        subArgs.push_back(arguments[i]->substitute(eng));
    }

    return new Invocation(expr->substitute(eng), subArgs);
}

Expression* Invocation::simplify(repl::ExecutionEngine* eng) {
    eng->pushFrame(arguments);
    Expression* simpl = expr->simplify(eng);
    Expression* sub = simpl->substitute(eng);
    Expression* ret = sub->simplify(eng);
    eng->popFrame();

    delete sub;
    delete simpl;

    return ret; 
}

std::string Invocation::toString() const {
    std::stringstream s;

    s << "(" << expr->toString() << ")(";
    for (int i = 0; i < arguments.size(); i++) {
        s << arguments[i]->toString();

        if (i != arguments.size() - 1) {
            s << ",";
        }
    }
    s << ")";

    return s.str();
}

bool Invocation::equals(repl::ExecutionEngine* eng, Expression* expr) {
    if (!expr) {
        return false;
    }

    Expression* simpl = simplify(eng);
    Expression* other = expr->simplify(eng);

    bool equality = false;
    if (Invocation* leftInvoc = dynamic_cast<Invocation*>(simpl)) {
        if (Invocation* rightInvoc = dynamic_cast<Invocation*>(other)) {
            equality = leftInvoc->getExpression()->equals(eng, rightInvoc->getExpression());

            std::vector<Expression*>& leftArgs = leftInvoc->getArguments();
            std::vector<Expression*>& rightArgs = rightInvoc->getArguments();

            equality &= leftArgs.size() == rightArgs.size();
            for (int i = 0; i < leftArgs.size() && equality; i++) {
                equality &= leftArgs[i]->equals(eng, rightArgs[i]);
            }
        }
    } else {
        equality = simpl->equals(eng, other);
    }

    delete simpl;
    delete other;

    return equality;
}

Expression* Invocation::clone() {
    return new Invocation(expr, arguments);
}
