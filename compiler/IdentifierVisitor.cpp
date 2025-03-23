#include "IdentifierVisitor.h"
#include <iostream>

IdentifierVisitor::IdentifierVisitor(SymbolTable* symboleTable) : ifccBaseVisitor()
{
    symTable = symboleTable;
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


antlrcpp::Any IdentifierVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
    visitChildren(ctx);
    symTable->checkIfEachIdUsed();
    symTable->checkIfEachIdInit();
    return 0;    
}