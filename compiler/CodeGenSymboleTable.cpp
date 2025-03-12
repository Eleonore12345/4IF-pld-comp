#include "CodeGenSymboleTable.h"

antlrcpp::Any CodeGenSymboleTable::visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    if(isInTable(varName)) {
        throw std::runtime_error("Redéclaration d'une variable");
    }
    symboleTable.push_back(Variable(varName,4));
    
    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any CodeGenSymboleTable::visitDeclarationV(ifccParser::DeclarationVContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if(isInTable(varName)) {
        throw std::runtime_error("Redéclaration d'une variable");
    }
    symboleTable.push_back(Variable(varName,4));
    
    this->visit(ctx->expr());

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