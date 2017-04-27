#include "dfxam.h"

using namespace dfxam::ast;

void testDerivative() {
    auto e = new E();
    auto x = new Variable('x');

    auto expr = new Power(e, x);
    std::cout << "Expression: " << expr << std::endl;

    auto derivative = expr->derivative(x);
    std::cout << "Derivative: " << derivative << std::endl;

    auto simplified = derivative->simplify();
    std::cout << "Simplified Derivative: " << simplified << std::endl;
}

void testSimplification() {
    auto x = new Variable('x');
    auto zero = new Constant(0);

    auto expr = new Sum(x, zero);
    std::cout << "Expression: " << expr << std::endl;

    auto simplified = expr->simplify();
    std::cout << "Simplified Derivative: " << simplified << std::endl;
}

int main() {
    testDerivative();
    return 0;
}
