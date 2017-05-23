#include "dfxam.h"

using namespace dfxam;

repl::Function::Function(std::string name, std::vector<ast::Expression*> inputs, ast::Expression* expr)
    : name(name), inputs(inputs), expr(expr) {}

repl::Function::Function(ast::Assignment* a)
    : name(a->getFunction()->getName()), 
      inputs(a->getFunction()->getArguments()), 
      expr(a->getExpression()) {}

ast::Expression* repl::Function::evaluate(repl::ExecutionEngine* eng) {
    return expr->simplify(eng);
}

std::string repl::Function::getName() const {
    return name;
}

void repl::ExecutionEngine::registerFunction(Function* f) {
    functions[f->getName()] = f;
}

repl::Function* repl::ExecutionEngine::retrieveFunction(std::string name) {
    auto find = functions.find(name);
    return find == functions.end() ? nullptr : find->second;
}

void repl::ExecutionEngine::deregisterFunction(std::string name) {
    Function* f;
    if (!(f = retrieveFunction(name))) {
        delete f;
    } 

    functions.erase(name);
}

void repl::ExecutionEngine::operator <<(std::string& raw) {
    repl::Lexer lexer(raw);

    std::vector<repl::Token> tokens;
    try {
         tokens = lexer.lex();
    } catch (std::string ex) {
        std::cout << ex;
        return;
    }

    repl::Parser parser(tokens);

    ast::Expression* expr;
    try {
        expr = parser.parse();
    } catch (std::string ex) {
        std::cout << ex;
    }

    execute(expr);
}

void repl::ExecutionEngine::execute(ast::Expression* expr) {
    if (ast::Assignment* a = dynamic_cast<ast::Assignment*>(expr)) {
        repl::Function* f = new repl::Function(a);

        registerFunction(f);
    } else {
        auto eval = expr->simplify(this);
        std::cout << eval; 

        delete eval;
        delete expr;
    }
}
