#include "dfxam.h"

using namespace dfxam::ast;

BinaryOperator::BinaryOperator(Expression* left, Expression* right)
    : left(left), right(right) {}

Expression* BinaryOperator::getLeft() const {
    return left;
}

Expression* BinaryOperator::getRight() const {
    return right;
}

BinaryOperator::~BinaryOperator() {
    delete left;
    delete right;
}
