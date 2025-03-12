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
        cout << "Variable " << varName << " already declared" << endl;
        error = true;
    } else {
        desc_identifier id;
        id.identifier = varName;
        id.index = (symTable->size() + 1) * (-4);
        symTable->addIdentifier(id);
        cout << "Variable " << varName << " declared" << endl;
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
        cout << "Variable " << varNameL << " already declared" << endl;
        error = true;
    }
    else if (symTable->getIndex(varNameR) == -1) 
    {
        cout << "Variable " << varNameR << " not declared" << endl;
        error = true;
    }
    else 
    {
        desc_identifier id;
        id.identifier = varNameL;
        id.index = (symTable->size() + 1) * (-4);
        symTable->addIdentifier(id);
        symTable->setUse(varNameR);
        cout << "Variable " << varNameL << " declared" << endl;
        cout << "Variable " << varNameR << " used" << endl;
    }
    return visitChildren(ctx);
}
                

antlrcpp::Any IdentifierVisitor::visitDefinitionC(ifccParser::DefinitionCContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) != -1) {
        cout << "Variable " << varName << " already declared" << endl;
        error = true;
    } else {
        desc_identifier id;
        id.identifier = varName;
        id.index = (symTable->size() + 1) * (-4);
        symTable->addIdentifier(id);
        cout << "Variable " << varName << " declared" << endl;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAffectationV(ifccParser::AffectationVContext *ctx)
{
    string varNameL = ctx->VAR(0)->getText();
    string varNameR = ctx->VAR(1)->getText();
    if (symTable->getIndex(varNameL) == -1) {
        cout << "Variable " << varNameL << " not declared" << endl;
        error = true;
    } 
    else if (symTable->getIndex(varNameR) == -1) {
        cout << "Variable " << varNameR << " not declared" << endl;
        error = true;
    } 
    else {
        symTable->setUse(varNameR);
        cout << "Variable " << varNameR << " used" << endl;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAffectationC(ifccParser::AffectationCContext *ctx)
{
    string varName = ctx->VAR()->getText();
    if (symTable->getIndex(varName) == -1) {
        cout << "Variable " << varName << " not declared" << endl;
        error = true;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
    visitChildren(ctx);
    if (!symTable->isEachIdUsed()) {
        error = true;
    }
    return 0;    
}