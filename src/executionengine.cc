#include "dfxam.h"

using namespace dfxam;

repl::Function::Function(std::string name, std::vector<char> inputs, ast::Expression* expr)
    : name(name), inputs(inputs), expr(expr) {}

ast::Expression* repl::Function::evaluate(repl::ExecutionEngine& eng) {
    return expr->simplify();
}

std::string repl::Function::getName() const {
    return name;
}

repl::ExecutionEngine::ExecutionEngine()
    : invokingFunction(nullptr) {}

void repl::ExecutionEngine::registerFunction(Function* f) {
    functions[f->getName()] = f;
}

repl::Function* repl::ExecutionEngine::retrieveFunction(std::string name) {
    auto find = functions.find(name);
    return find == functions.end() ? nullptr : find->second;
}

void repl::ExecutionEngine::deregisterFunction(std::string name) {
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
}
