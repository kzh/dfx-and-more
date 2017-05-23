#include "dfxam.h"

using namespace dfxam::ast;

Function::Function(std::string f)
    : name(f) {}

Function::Function(std::string f, std::vector<Expression*> inputs)
    : name(f), inputs(inputs) {}

std::string Function::getName() const {
    return name;
}

std::vector<Expression*> Function::getArguments() const {
    return inputs;
}

Expression* Function::derivative(Function* respect) {
    if (respect->getName() == name) {
        return new Constant(1);
    } else {
        return new Differentiation(this, respect);
    }
}

Expression* Function::simplify(repl::ExecutionEngine* eng) {
//    std::cout << "Simplifying: " << this << std::endl;
    repl::Function* f;
    if (!(f = eng->retrieveFunction(name))) {
        return clone();
    }

    return f->evaluate(eng);
}

std::string Function::toString() const {
    return name;
}

bool Function::equals(repl::ExecutionEngine* eng, Expression* expr) {
    Function* v = nullptr;
    if (!(v = dynamic_cast<Function*>(expr))) {
        return false;
    }

    return v->getName() == getName(); 
}

Expression* Function::clone() {
    return new Function(name, inputs);
}
