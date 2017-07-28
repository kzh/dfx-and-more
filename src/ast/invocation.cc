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

    std::cout << "broke1" << std::endl;
    Expression* simpl = expr->simplify(eng);
    std::cout << "broke2" << std::endl;
    Expression* sub = simpl->substitute(eng);
    std::cout << "broke3" << std::endl;
    Expression* ret = sub->simplify(eng);
    std::cout << "broke4" << std::endl;

    eng->popFrame();

    delete sub;
    delete simpl;

    return ret; 
}

Function* Invocation::getVar() {
    for (int i = 0; i < arguments.size(); i++) {
        if (Function* var = arguments[i]->getVar()) {
            return var;
        }
    }

    return nullptr;
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
    return false;
}

Expression* Invocation::clone() {
    std::vector<Expression*> args;
    for (int i = 0; i < arguments.size(); i++) {
        args.push_back(arguments[i]->clone());
    }

    return new Invocation(expr->clone(), args);
}

Invocation::~Invocation() {
    delete expr;
    for (int i = 0; i < arguments.size(); i++) {
        delete arguments[i];
    }
}
