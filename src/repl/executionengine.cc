#include "dfxam.h"

using namespace dfxam;

repl::Function::Function(std::string name, ast::Expression* expr) 
    : name(name), expr(expr) {}

repl::Function::Function(ast::Assignment* a) {
    ast::Expression* ident = a->getDeclaration()->getExpression();
    if (ast::Function* func = dynamic_cast<ast::Function*>(ident)) {
       name = func->getName();
    }

    std::vector<ast::Expression*> ins = a->getDeclaration()->getArguments();
    for (auto it = ins.begin(); it != ins.end(); it++) {
        ast::Function* func = dynamic_cast<ast::Function*>(*it);
        if (func) {
            this->inputs.push_back(func->getName());
        }
    }

    expr = a->getExpression();
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

repl::Frame::Frame(std::vector<ast::Expression*> args) 
    : arguments(args) {}

bool repl::Frame::bindParameters(std::vector<std::string> params) {
    if (params.size() != arguments.size()) {
        return false;
    }

    for (int i = 0; i < params.size(); i++) {
        parameters[params[i]] = i;
    }

    return true;
}

ast::Expression* repl::Frame::getArg(std::string name) {
    auto find = parameters.find(name);
    return find == parameters.end() ? nullptr : arguments[find->second];
}

void repl::ExecutionEngine::registerFunction(repl::Function* f) {
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

ast::Expression* repl::ExecutionEngine::getFrameArg(std::string name) {
    if (callStack.empty()) {
        return nullptr;
    }

    Frame& f = callStack.top();
    return f.getArg(name);
}

bool repl::ExecutionEngine::bindFrameParameters(std::vector<std::string> params) {
    if (callStack.empty()) {
        return false;
    }

    Frame& f = callStack.top();
    return f.bindParameters(params);
}

void repl::ExecutionEngine::pushFrame(std::vector<ast::Expression*> args) {
    Frame f(args);
    callStack.push(f); 
}

void repl::ExecutionEngine::popFrame() {
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
        std::cout << "Registering function: " << f->getName() << std::endl;

        registerFunction(f);
    } else {
        std::cout << "Evaluating expression: " << expr << std::endl;
        auto eval = expr->simplify(this);
        std::cout << eval; 

        delete eval;
        delete expr;
    }
}
