#include "CodeGenSymbolTable.h"

antlrcpp::Any CodeGenSymbolTable::visitDeclarationV(ifccParser::DeclarationVContext *ctx)
{
    std::string varName = ctx->VAR()->getText();

    if(isInTable(varName))
    {
        throw std::runtime_error("Variable " + varName + " already declared");
    }

    int index = (symbolTable.size()+1) * 4;
    symbolTable.push_back(Variable(varName, index));

    visitChildren(ctx);
    
    return 0;
}

antlrcpp::Any CodeGenSymbolTable::visitDeclarationVaV(ifccParser::DeclarationVaVContext *ctx)
{
    std::string varNameGauche = ctx->VAR(0)->getText();
    std::string varNameDroite = ctx->VAR(1)->getText();

    if(isInTable(varNameGauche))
    {
        throw std::runtime_error("Variable " + varNameGauche + " already declared");
    }

    if(not(isInTable(varNameDroite)))
    {
        throw std::runtime_error("Variable " + varNameDroite + " not declared");
    }

    int index = (symbolTable.size()+1) * 4;
    symbolTable.push_back(Variable(varNameGauche, index));

    visitChildren(ctx);
    
    return 0;
}

antlrcpp::Any CodeGenSymbolTable::visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx)
{
    std::string varName = ctx->VAR()->getText();

    if(isInTable(varName))
    {
        throw std::runtime_error("Variable " + varName + " already declared");
    }

    int index = (symbolTable.size()+1) * 4;
    symbolTable.push_back(Variable(varName, index));

    visitChildren(ctx);
    
    return 0;
}


antlrcpp::Any CodeGenSymbolTable::visitAffectationVaV(ifccParser::AffectationVaVContext *ctx)
{
    std::string varNameGauche = ctx->VAR(0)->getText();
    std::string varNameDroite = ctx->VAR(1)->getText();

    if(not(isInTable(varNameGauche)))
    {
        throw std::runtime_error("Variable " + varNameGauche + " not declared");
    }

    if(not(isInTable(varNameDroite)))
    {
        throw std::runtime_error("Variable " + varNameDroite + " not declared");
    }

    visitChildren(ctx);
    
    return 0;
}

antlrcpp::Any CodeGenSymbolTable::visitAffectationVaC(ifccParser::AffectationVaCContext *ctx)
{
    std::string varName = ctx->VAR()->getText();

    if(not(isInTable(varName)))
    {
        throw std::runtime_error("Variable " + varName + " not declared");
    }

    visitChildren(ctx);
    
    return 0;
}


bool CodeGenSymbolTable::isInTable(std::string varName) {
    for (size_t i = 0; i < symbolTable.size(); i++) {
        if(varName.compare(symbolTable[i].name) == 0) {
            return true;
        }
    }
    return false;
}
