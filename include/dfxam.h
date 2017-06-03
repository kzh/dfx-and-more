#ifndef DFXAM
#define DFXAM

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <tuple>

namespace dfxam {
namespace repl {
    class ExecutionEngine;
} // repl

namespace ast {
class Function;
class Expression {
    public:
        virtual Expression* derivative(Function* respect) = 0;
        virtual Expression* simplify(repl::ExecutionEngine* eng) = 0;
        virtual std::string toString() const = 0;

        virtual bool equals(repl::ExecutionEngine* eng, Expression* expr) = 0;
        virtual Expression* clone() = 0;

        virtual bool isConstant() const;

        virtual ~Expression() {};
};

class Constant: public Expression {
    public:
        Constant(double val);
        double getValue() const;

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
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

class Function : public Expression {
    public:
        Function(std::string f);
        Function(std::string f, std::vector<Expression*> inputs);

        std::string getName() const;
        std::vector<Expression*> getArguments() const;

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

    private:
        std::string name;
        std::vector<Expression*> inputs;
};

class Differentiation : public Expression {
    public:
        Differentiation(Expression* expr, Function* respect);
        Expression* getExpression() const;
        Function* getRespect() const;

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

        ~Differentiation() override;

    private:
        Expression* expr;
        Function* respect;
};

class BinaryOperator : public Expression {
    public:
        BinaryOperator();
        BinaryOperator(Expression* left, Expression* right);

        void setLeft(Expression* l);
        void setRight(Expression* r);

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

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Log : public BinaryOperator {
    public:
        Log(Expression* left, Expression* right);

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Sum : public BinaryOperator {
    public:
        Sum(Expression* left, Expression* right);

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Difference : public BinaryOperator {
    public:
        Difference(Expression* left, Expression* right);

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Product : public BinaryOperator {
    public:
        Product(Expression* left, Expression* right);

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

class Quotient : public BinaryOperator {
    public:
        Quotient(Expression* left, Expression* right);

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;
};

// Special instruction for ExecutionEngine
// to store function in memory
class Assignment : public Expression {
    public:
        Assignment(Function* f, Expression* expr);

        Function* getFunction() const;
        Expression* getExpression() const;

        Expression* derivative(Function* respect) override;
        Expression* simplify(repl::ExecutionEngine* eng) override;
        std::string toString() const override;

        bool equals(repl::ExecutionEngine* eng, Expression* expr) override;
        Expression* clone() override;

        ~Assignment();

    private:
        Function* f;
        Expression* expr;
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

enum class Associativity { LEFT, RIGHT };

typedef std::tuple<Associativity, int> OperatorInfo;
static std::unordered_map<std::string, OperatorInfo> OP_INFO = {
    {"+", std::make_tuple(Associativity::LEFT, 1)},
    {"-", std::make_tuple(Associativity::LEFT, 1)},
    {"*", std::make_tuple(Associativity::LEFT, 2)},
    {"/", std::make_tuple(Associativity::LEFT, 2)},
    {"^", std::make_tuple(Associativity::RIGHT, 3)}
};

class Parser {
    public:
        Parser(std::vector<Token> tokens);
        ast::Expression* parse();

    private:
        bool match(TokenType t, std::string s, int ahead);
        Token& peek();
        void consume();

        ast::Expression* parseAssignment();

        ast::Expression* parseExpression(int minPrecedence = 0);
        ast::Expression* parseAtom();
        ast::Expression* parseBinop(std::string op, ast::Expression* lhs, ast::Expression* rhs);

        ast::Expression* parseInvocation();
        ast::Expression* parseConstant();

        std::vector<Token> tokens;
        int index;

};

class ExecutionEngine;
class Function {
    public:
        Function(std::string name, std::vector<ast::Expression*> inputs, ast::Expression* expr);
        Function(ast::Assignment* a); 
        ast::Expression* evaluate(ExecutionEngine* eng);
        std::string getName() const;

    private:
        std::string name;
        std::vector<ast::Expression*> inputs;
        ast::Expression* expr;
};

class ExecutionEngine {
    public:
        void registerFunction(Function* f);
        Function* retrieveFunction(std::string name);
        void deregisterFunction(std::string name);

        void operator <<(std::string& expr);
        void execute(ast::Expression* expr);

    private:
        std::unordered_map<std::string, Function*> functions;
        std::stack<Function*> callStack;
};

} // repl
} // dfaxm

std::ostream& operator <<(std::ostream& out, const dfxam::ast::Expression* expr);

#endif
