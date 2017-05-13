#include "dfxam.h"

using namespace dfxam::ast;

Function::Function(std::string v)
    : name(v) {}

std::string Function::getName() const {
    return name;
}

Expression* Function::derivative(Function* respect) {
    if (respect->getName() == name) {
        return new Constant(1);
    } else {
        return new Differentiation(this, respect);
    }
}

Expression* Function::simplify() {
//    std::cout << "Simplifying: " << this << std::endl;

    return clone();
}

std::string Function::toString() const {
    return name;
}

bool Function::equals(Expression* expr) {
    Function* v = nullptr;
    if (!(v = dynamic_cast<Function*>(expr))) {
        return false;
    }

    return v->getName() == getName(); 
}

Expression* Function::clone() {
    return new Function(getName());
}
