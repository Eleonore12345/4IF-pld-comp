#include "IdentifierVisitor.h"
#include <iostream>

IdentifierVisitor::IdentifierVisitor(SymbolTable* symboleTable, map<string,int> * fonctionsDef) : ifccBaseVisitor()
{
    symTable = symboleTable;
    fonctionsDefined = fonctionsDef;
    error = false;
}

antlrcpp::Any IdentifierVisitor::visitInitDecla(ifccParser::InitDeclaContext *ctx) {
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) != -1) {
        std::string erreur = "Variable " + varName + " already declared\n";
        throw std::runtime_error(erreur);
        error = true;
    } else {
        desc_identifier id;
        id.identifier = varName;
        id.offset = (symTable->size() + 1) * 4;
        if(ctx->expr()) {
            id.init = true;
        }
        symTable->addIdentifier(id);
    }
    return visitChildren(ctx);
}

bool IdentifierVisitor::getError() {
    return error;
}

antlrcpp::Any IdentifierVisitor::visitAffectation(ifccParser::AffectationContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) == -1) {
        std::string erreur = "Variable " + varName + "not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    }
    symTable->setInit(varName);
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitVariableSimple(ifccParser::VariableSimpleContext *ctx) {
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) == -1) {
        std::string erreur = "Variable " + varName + " not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    }
    else{
        if(symTable->getInitStatus(varName) == 0) {
            cerr << "WARNING : variable " << varName << " used but not initialized" << endl;
        }
        symTable->setUse(varName);
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpMultDiv(ifccParser::OpMultDivContext *ctx) {
    antlrcpp::Any result = visit(ctx->expr(1));
    if (result.is<bool>() && result.as<bool>()) {
        cerr << "WARNING : division by zero" << endl;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitConstante(ifccParser::ConstanteContext *ctx) {
    std::string constant = ctx->CONST()->getText();
    int val;
    if (constant[0] == '\'') {
        val = (int) constant[1];
    }
    else {
        val = stol(constant);
    }
    if(val == 0) {
        return antlrcpp::Any(true);
    }
    return antlrcpp::Any(false);
}

antlrcpp::Any IdentifierVisitor::visitFunctionCall(ifccParser::FunctionCallContext *ctx) {
    std::string funcName = ctx->VAR()->getText();
    if(fonctionsDefined->find(funcName) == fonctionsDefined->end()) {
        std::cerr << "WARNING : implicit declaration of function " << funcName << std::endl;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
    visitChildren(ctx);
    if(fonctionsDefined->find("main") == fonctionsDefined->end()) {
        std::string erreur = "undefined reference to main\n";
        throw std::runtime_error(erreur);
        error = true;
    }
    symTable->checkIfEachIdUsed();
    symTable->checkIfEachIdInit();
    return 0;    
}

antlrcpp::Any IdentifierVisitor::visitDefFunc(ifccParser::DefFuncContext * ctx) {
    std::string funcName = ctx->VAR()->getText();
    int nbParams = visit(ctx->params());
    (*fonctionsDefined)[funcName] = nbParams;
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitNoParam(ifccParser::NoParamContext *ctx)
{
    return 0;
}
                
antlrcpp::Any IdentifierVisitor::visitWithParams(ifccParser::WithParamsContext *ctx)
{
    return ctx->expr().size();
}