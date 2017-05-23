#include "dfxam.h"

using namespace dfxam::ast;

void testDerivative() {
    dfxam::repl::ExecutionEngine engine;

    auto e = new E();                                // e

    auto two = new Constant(2);                      // 2
    auto x = new Function("x");                      // x
    auto twox = new Product(two, x);                 // 2x

    auto etotwox = new Power(e, twox);               // e ^ (2x)

    auto five = new Constant(5);                     // 5
    auto fiveoverx = new Quotient(five, x->clone()); // 5 / x

    auto expr = new Sum(etotwox, fiveoverx);         // e ^ (2x) + (5/x)

    std::cout << "Expression: " << expr << std::endl;

    auto derivative = expr->derivative(x);
    std::cout << "Derivative: " << derivative << std::endl;

    auto simplified = derivative->simplify(&engine);
    std::cout << "Simplified Derivative: " << simplified << std::endl;
}

void testSimplification() {
    dfxam::repl::ExecutionEngine engine;

    auto x = new Function("x");
    auto zero = new Constant(0);

    auto expr = new Sum(x, zero);
    std::cout << "Expression: " << expr << std::endl;

    auto simplified = expr->simplify(&engine);
    std::cout << "Simplified: " << simplified << std::endl;
}

int main() {
    testDerivative();
    return 0;
}
