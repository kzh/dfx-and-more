#include "dfxam.h"

using namespace dfxam;

repl::Function::Function(std::string name, ast::Expression* expr) 
    : name(name), expr(expr) {}

repl::Function::Function(ast::Assignment* a)
    : Function(a->getFunction()->getName(), a->getExpression()) {
    std::vector<ast::Expression*> ins = a->getFunction()->getArguments();
    for (auto it = ins.begin(); it != ins.end(); it++) {
        ast::Function* func = dynamic_cast<ast::Function*>(*it);
        if (func) {
            this->inputs.push_back(func->getName());
        }
    }
}

ast::Expression* repl::Function::getExpression() const {
    return expr;
}

std::string repl::Function::getName() const {
    return name;
}

std::vector<std::string> repl::Function::getInputs() const {
    return inputs;
}

void repl::ExecutionEngine::registerFunction(repl::Function* f) {
    functions[f->getName()] = f;
}

repl::Function* repl::ExecutionEngine::locateFunction(std::string name) {
    auto find = functions.find(name);
    return find == functions.end() ? nullptr : find->second;
}

repl::Function* repl::ExecutionEngine::retrieveFunction(std::string name) {
    Function* f;
    if ((f = locateFunction(getScopedName(name))) ||
        (f = locateFunction(name))) {
        return f;
    }

    return nullptr;
}

void repl::ExecutionEngine::deregisterFunction(std::string name) {
    Function* f;
    if (!(f = retrieveFunction(name))) {
        delete f;
    } 

    functions.erase(name);
}

std::string repl::ExecutionEngine::getScopedName(std::string name) {
    if (callStack.size() == 0) {
        return name;
    } else {
        Function* f = callStack.top();
        return f->getName() + "_" + name;
    }
}

void repl::ExecutionEngine::pushCall(Function* f) {
    callStack.push(f); 
}

void repl::ExecutionEngine::popCall() {
    callStack.pop();
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
