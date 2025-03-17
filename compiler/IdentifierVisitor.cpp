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
        id.offset = (symTable->size() + 1) * 4;
        symTable->addIdentifier(id);
    }
    return visitChildren(ctx);
}

bool IdentifierVisitor::getError() {
    return error;
}

                

antlrcpp::Any IdentifierVisitor::visitDefinition(ifccParser::DefinitionContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) != -1) {
        std::string erreur = "Variable " + varName + " already declared\n";
        throw std::runtime_error(erreur);
        error = true;
    } else {
        desc_identifier id;
        id.identifier = varName;
        id.offset = (symTable->size() + 1) * 4;
        symTable->addIdentifier(id);
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAffectation(ifccParser::AffectationContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) == -1) {
        std::string erreur = "Variable " + varName + "not declared\n";
        throw std::runtime_error(erreur);
        error = true;
    }
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
        symTable->setUse(varName);
    }
    return visitChildren(ctx);
}



antlrcpp::Any IdentifierVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
    visitChildren(ctx);
    if (!symTable->isEachIdUsed()) {
        //TO-DO : METTRE UN WARNING ET NON UNE ERREUR
    }
    return 0;    
}