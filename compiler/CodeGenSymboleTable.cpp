#include "CodeGenSymboleTable.h"

antlrcpp::Any CodeGenSymboleTable::visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    if(isInTable(varName)) {
        throw std::runtime_error("Redéclaration d'une variable");
    }
    int index = (symboleTable.size()+1)*4;
    symboleTable.push_back(Variable(varName,index));
    
    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenSymboleTable::visitDeclarationVaV(ifccParser::DeclarationVaVContext *ctx) {
    std::string varName0 = ctx->VAR(0)->getText();
    std::string varName1 = ctx->VAR(1)->getText();
    if(!isInTable(varName1)) {
        throw std::runtime_error("Redéclaration d'une variable");
    }
    if(isInTable(varName0)) {
        throw std::runtime_error("Redéclaration d'une variable");
    }
    int index = (symboleTable.size()+1)*4;
    symboleTable.push_back(Variable(varName0,index));
    
    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenSymboleTable::visitDeclarationV(ifccParser::DeclarationVContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if(isInTable(varName)) {
        throw std::runtime_error("Redéclaration d'une variable");
    }
    int index = (symboleTable.size()+1)*4;
    symboleTable.push_back(Variable(varName,index));
    
    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenSymboleTable::visitAffectationVaC(ifccParser::AffectationVaCContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if(!isInTable(varName)) {
        throw std::runtime_error("Variable non déclarée");
    }
    
    visitChildren(ctx);

    return 0;
}


bool CodeGenSymboleTable::isInTable(std::string varName) {
    for (size_t i = 0; i < symboleTable.size(); i++) {
        if(varName.compare(symboleTable[i].name) == 0) {
            return true;
        }
    }
    return false;
}