#include "dfxam.h"

std::ostream& operator <<(std::ostream& out, const dfxam::ast::Expression* expr) {
    return out << expr->toString();
}

int main() {
    auto three = new dfxam::ast::Constant(3);
    auto five = new dfxam::ast::Constant(5);
    auto x = new dfxam::ast::Variable('x');

    auto product = new dfxam::ast::Product(five, x);
    auto p = new dfxam::ast::Power(three, product);

    std::cout << "The derivative of (3 ^ (5x)) is:" << std::endl;
    std::cout << p->derivative(x) << std::endl;
}
