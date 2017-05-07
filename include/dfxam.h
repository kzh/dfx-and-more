#ifndef DFXAM
#define DFXAM

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace dfxam {
namespace ast {

class Variable;
class Expression {
    public:
        virtual Expression* derivative(Variable* respect) = 0;
        virtual Expression* simplify() = 0;
        virtual std::string toString() const = 0;

        virtual bool equals(Expression* expr) = 0;
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

        bool equals(Expression* expr) override;
        Expression* clone() override;

        bool isConstant() const override;
        static bool isConstantValue(Expression* expr, int val);

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
        Variable(std::string v);
        std::string getVariable() const;

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
        Expression* clone() override;

    private:
        std::string variable;
};

class Differentiation : public Expression {
    public:
        Differentiation(Expression* expr, Variable* respect);
        Expression* getExpression() const;
        Variable* getRespect() const;

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
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

        bool equals(Expression* expr) override;
        Expression* clone() override;
};

class Log : public BinaryOperator {
    public:
        Log(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
        Expression* clone() override;
};

class Sum : public BinaryOperator {
    public:
        Sum(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
        Expression* clone() override;
};

class Difference : public BinaryOperator {
    public:
        Difference(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
        Expression* clone() override;
};

class Product : public BinaryOperator {
    public:
        Product(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
        Expression* clone() override;
};

class Quotient : public BinaryOperator {
    public:
        Quotient(Expression* left, Expression* right);

        Expression* derivative(Variable* respect) override;
        Expression* simplify() override;
        std::string toString() const override;

        bool equals(Expression* expr) override;
        Expression* clone() override;
};

} // ast

namespace repl {

enum class TokenType { IDENTIFIER, NUMBER, OPERATOR, SEPARATOR };

class Token {
    public:
        Token(TokenType type, std::string contents);
        bool match(TokenType type, std::string contents);

        std::string getContents() const;
        TokenType getType() const;

    private:
        TokenType type;
        std::string contents;
};

class Lexer {
    public:
        Lexer(std::string s);

        std::vector<Token> lex();
        static bool isOperator(char c);
        static bool isSeparator(char c);
        static bool isLetter(char c);
        static bool isNumber(char c);

    private:
        char& peek();
        void consume();

        void lexIdentifier();
        void lexNumber();
        void lexSeparator();
        void lexOperator();

        std::string s;
        int index;
        std::vector<Token> tokens;
};

class Parser {
    public:

    private:
};

class Function {
    public:
        Function(std::string name, std::vector<char> inputs, ast::Expression* expr);
        std::string getName() const;

    private:
        std::string name;
        std::vector<char> inputs;
        ast::Expression* expr;
};

class ExecutionEngine {
    public:
        void registerFunction(Function* f);
        Function* retrieveFunction(std::string name);
        void deregisterFunction(std::string name);

        void operator <<(std::string& expr);

    private:
        std::unordered_map<std::string, Function*> functions;
};

} // repl
} // dfaxm

std::ostream& operator <<(std::ostream& out, const dfxam::ast::Expression* expr);

#endif
