#include "dfxam.h"

void testDerivative() {
    auto five = new dfxam::ast::Constant(5);
    auto x = new dfxam::ast::Variable('x');

    auto expr = new dfxam::ast::Power(x, five);
    std::cout << "Expression: " << expr << std::endl;

    auto derivative = expr->derivative(x);
    std::cout << "Derivative: " << derivative << std::endl;

    auto simplified = derivative->simplify();
    std::cout << "Simplified Derivative: " << simplified << std::endl;
}

void testSimplification() {
    auto x = new dfxam::ast::Variable('x');
    auto zero = new dfxam::ast::Constant(0);

    auto expr = new dfxam::ast::Sum(x, zero);
    std::cout << "Expression: " << expr << std::endl;

    auto simplified = expr->simplify();
    std::cout << "Simplified Derivative: " << simplified << std::endl;
}

int main() {
    testDerivative();
    return 0;
}
