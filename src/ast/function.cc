#include "dfxam.h"

using namespace dfxam::ast;

Function::Function(std::string name)
    : name(name) {}

std::string Function::getName() const {
    return name;
}

Expression* Function::derivative(repl::ExecutionEngine* eng, Function* respect) {
    if (equals(eng, respect)) {
        return new Constant(1);
    }

    if (repl::Function* f = eng->retrieveFunction(name)) {
        eng->bindFrameParameters(f->getInputs());
        return f->getExpression()->derivative(eng, respect);
    }

    return new Differentiation(clone(), respect);
}

Expression* Function::substitute(repl::ExecutionEngine* eng) {
    if (Expression* arg = eng->getFrameArg(name)) {
        return arg->clone();
    }

    return clone();
}

Expression* Function::simplify(repl::ExecutionEngine* eng) {
    if (repl::Function* f = eng->retrieveFunction(name)) {
        eng->bindFrameParameters(f->getInputs());
        return f->getExpression()->clone();
    }

    return clone();
}

Function* Function::getVar(repl::ExecutionEngine* eng) {
    if (repl::Function* f = eng->retrieveFunction(name)) {
        std::vector<std::string> params = f->getInputs();
        if (!params.empty()) {
            return new Function(params[0]);
        }
    }

    return new Function(name);
}

std::string Function::toString() const {
    return name;
}

bool Function::equals(repl::ExecutionEngine* eng, Expression* expr) {
    if (Function* other = dynamic_cast<Function*>(expr)) {
        return name == other->getName();
    }

    return false;
}

Expression* Function::clone() {
    return new Function(name);
}
