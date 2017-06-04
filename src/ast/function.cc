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
    repl::Function* f;
    if (!(f = eng->retrieveFunction(name))) {
        return clone();
    }

    // simplify the inputs in regards to current call frame
    std::vector<ast::Expression*> simpInputs; 
    for (int i = 0; i < inputs.size(); i++) {
        simpInputs.push_back(inputs[i]->simplify(eng));
    }

    // add function call to stack
    eng->pushCall(f);

    // bind function inputs to function call frame
    std::vector<std::string> inputNames = f->getInputs();
    for (int i = 0; i < inputs.size(); i++) {
        std::string n = eng->getScopedName(inputNames[i]);
        repl::Function* temp = new repl::Function(n, simpInputs[i]);
        eng->registerFunction(temp);
    }

    // eval
    ast::Expression* val = f->getExpression()->simplify(eng);

    // unbind function inpus to function call frame
    for (int i = 0; i < inputs.size(); i++) {
        std::string n = eng->getScopedName(inputNames[i]);
        eng->deregisterFunction(n);
    }

    // pop function call from stack
    eng->popCall();

    return val;
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
