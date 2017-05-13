#include "dfxam.h"

using namespace dfxam::ast;

BinaryOperator::BinaryOperator()
    : left(nullptr), right(nullptr) {}

BinaryOperator::BinaryOperator(Expression* left, Expression* right)
    : left(left), right(right) {}

void BinaryOperator::setLeft(Expression* l) {
    if (left != nullptr) {
        return;
    }

    left = l;
}

void BinaryOperator::setRight(Expression* r) {
    if (right != nullptr) {
        return;
    }

    right = r;
}

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
