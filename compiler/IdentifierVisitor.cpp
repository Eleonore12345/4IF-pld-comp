#include "IdentifierVisitor.h"
#include <iostream>

IdentifierVisitor::IdentifierVisitor(SymbolTable* symboleTable) : ifccBaseVisitor()
{
    symTable = symboleTable;
    error = false;
}

antlrcpp::Any IdentifierVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx) 
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) != -1) {
        std::string erreur = "Variable " + varName + " already declared\n";
        throw std::runtime_error(erreur);
        error = true;
    } else {
        desc_identifier id;
        id.identifier = varName;
        id.index = (symTable->size() + 1) * (-4);
        symTable->addIdentifier(id);
    }
    return visitChildren(ctx);
}

bool IdentifierVisitor::getError() {
    return error;
}

antlrcpp::Any IdentifierVisitor::visitDefinitionV(ifccParser::DefinitionVContext *ctx)
{
    string varNameL = ctx->VAR(0)->getText();
    string varNameR = ctx->VAR(1)->getText();
    if (symTable->getIndex(varNameL) != -1) 
    {
        std::string erreur = "Variable " + varNameL + " already declared\n";
        throw std::runtime_error(erreur);
        error = true;
    }
    else if (symTable->getIndex(varNameR) == -1) 
    {
        std::string erreur = "Variable " + varNameR + " not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    }
    else 
    {
        desc_identifier id;
        id.identifier = varNameL;
        id.index = (symTable->size() + 1) * (-4);
        symTable->addIdentifier(id);
        symTable->setUse(varNameR);
    }
    return visitChildren(ctx);
}
                

antlrcpp::Any IdentifierVisitor::visitDefinitionC(ifccParser::DefinitionCContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) != -1) {
        std::string erreur = "Variable " + varName + " already declared\n";
        throw std::runtime_error(erreur);
        error = true;
    } else {
        desc_identifier id;
        id.identifier = varName;
        id.index = (symTable->size() + 1) * (-4);
        symTable->addIdentifier(id);
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAffectationV(ifccParser::AffectationVContext *ctx)
{
    string varNameL = ctx->VAR(0)->getText();
    string varNameR = ctx->VAR(1)->getText();
    if (symTable->getIndex(varNameL) == -1) {
        std::string erreur = "Variable " + varNameL + " not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    } 
    else if (symTable->getIndex(varNameR) == -1) {
        std::string erreur = "Variable " + varNameR + " not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    } 
    else {
        symTable->setUse(varNameR);
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAffectationC(ifccParser::AffectationCContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) == -1) {
        std::string erreur = "Variable " + varName + " not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
    visitChildren(ctx);
    if (!symTable->isEachIdUsed()) {
        error = false;
        //TO-DO : METTRE UN WARNING ET NON UNE ERREUR
    }
    return 0;    
}