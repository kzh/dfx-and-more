#ifndef DFXAM
#define DFXAM

#include <string>
#include <sstream>
#include <iostream>

namespace dfxam {
namespace ast {

class Variable;
class Expression {
    public:
        virtual Expression* derivative(Variable* respect) = 0;
        virtual Expression* simplify() = 0;
        virtual std::string toString() const = 0;

        virtual Expression* clone() = 0;

        virtual bool isConstant() const;

        virtual ~Expression() {};
};

class Constant: public Expression {
    public:
        Constant(double val);
        double getValue() const;

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;

        bool isConstant() const override;

    private:
        double value;
};

class E : public Constant {
    public:
        E();

        std::string toString() const override; 

        Expression* clone() override;
};

class Variable : public Expression {
    public:
        Variable(char v);
        char getVariable() const;

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;

    private:
        char variable;
};

class Differentiation : public Expression {
    public:
        Differentiation(Expression* expr, Variable* respect);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;

        ~Differentiation() override;

    private:
        Expression* expr;
        Variable* respect;
};

class BinaryOperator : public Expression {
    public:
        BinaryOperator(Expression* left, Expression* right);

        Expression* getLeft() const;
        Expression* getRight() const;

        ~BinaryOperator() override;

    private:
        Expression* left;
        Expression* right;
};

class Power : public BinaryOperator {
    public:
        Power(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;
};

class Log : public BinaryOperator {
    public:
        Log(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;
};

class Sum : public BinaryOperator {
    public:
        Sum(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;
};

class Difference : public BinaryOperator {
    public:
        Difference(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;
};

class Product : public BinaryOperator {
    public:
        Product(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;
};

class Quotient : public BinaryOperator {
    public:
        Quotient(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        Expression* clone() override;
};

} // ast
} // dfaxm

std::ostream& operator <<(std::ostream& out, const dfxam::ast::Expression* expr);

#endif
