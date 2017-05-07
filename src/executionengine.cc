#include "dfxam.h"

using namespace dfxam;

repl::Function::Function(std::string name, std::vector<char> inputs, ast::Expression* expr)
    : name(name), inputs(inputs), expr(expr) {}

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
    functions.erase(name);
}

std::string printTokenType(repl::TokenType type) {
    switch (type) {
    case repl::TokenType::IDENTIFIER:
        return "Identifier";
    case repl::TokenType::NUMBER:
        return "Number";
    case repl::TokenType::OPERATOR:
        return "Operator";
    case repl::TokenType::SEPARATOR:
        return "Separator";
    }

    return "";
}

void repl::ExecutionEngine::operator <<(std::string& expr) {
    repl::Lexer lexer(expr);

    std::vector<repl::Token> tokens;
    try {
         tokens = lexer.lex();
    } catch (std::string ex) {
        std::cout << ex;
        return;
    }

    for (auto it = tokens.begin(); it != tokens.end(); it++) {
        std::cout << printTokenType((*it).getType()) << ": ";
        std::cout << (*it).getContents();
        std::cout << std::endl;
    }
}
