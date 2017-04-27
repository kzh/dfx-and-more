#include "dfxam.h"

using namespace dfxam::ast;

bool Expression::isConstant() const {
    return false;
}

std::ostream& operator <<(std::ostream& out, const Expression* expr) {
    return out << expr->toString();
}
