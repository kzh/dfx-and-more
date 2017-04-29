#include "dfxam.h"

using namespace dfxam::ast;

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

bool Variable::equals(Expression* expr) {
    Variable* v = nullptr;
    if (!(v = dynamic_cast<Variable*>(expr))) {
        return false;
    }

    return v->getVariable() == getVariable(); 
}

Expression* Variable::clone() {
    return new Variable(getVariable());
}
