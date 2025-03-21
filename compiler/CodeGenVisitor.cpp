#include "CodeGenVisitor.h"
#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(SymbolTable* symboleTable, CFG* c) : ifccBaseVisitor()
{
    symbolTable = symboleTable;
    cfg = c;
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    //ajout du premier bloc de base dans le cfg
    BasicBlock* bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(bb);
    cfg->current_bb = bb;

    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDefinition(ifccParser::DefinitionContext *ctx) {

    string expr_content = visit(ctx->expr());
    // pour gérer si on a une constante ou une variable à droite
    VariableOrConstante(ctx->VAR()->getText(), expr_content);
   
    visit(ctx->instr());
    return 0;
    
}

antlrcpp::Any CodeGenVisitor::visitParentheses(ifccParser::ParenthesesContext *ctx) {
    visitChildren(ctx);
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
    visit(ctx->instr());
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectation(ifccParser::AffectationContext *ctx) {

    string expr_content = visit(ctx->expr());
    // pour gérer si on a une constante ou une variable à droite
    VariableOrConstante(ctx->VAR()->getText(), expr_content);

    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitVariableSimple(ifccParser::VariableSimpleContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int index = symbolTable->getOffset(varName);

    return varName;
}

antlrcpp::Any CodeGenVisitor::visitConstante(ifccParser::ConstanteContext *ctx) {
    std::string constant = ctx->CONST()->getText();
    return constant;
}

antlrcpp::Any CodeGenVisitor::visitOpAddSub(ifccParser::OpAddSubContext *ctx) {
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = "tmp" + to_string(symbolTable->size());
    desc_identifier idG;
    idG.identifier = nameVarTmpG;
    idG.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idG);
    VariableOrConstante(nameVarTmpG, operandeG);
    
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = "tmp" + to_string(symbolTable->size());
    desc_identifier idD;
    idD.identifier = nameVarTmpD;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);
    VariableOrConstante(nameVarTmpD, operandeD);

    std::string op = ctx->OP->getText();
    if(op == "+") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::add, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::sub, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    }
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpMultDiv(ifccParser::OpMultDivContext *ctx) {
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = "tmp" + symbolTable->size();
    desc_identifier idG;
    idG.identifier = nameVarTmpG;
    idG.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idG);
    VariableOrConstante(nameVarTmpG, operandeG);
    
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = "tmp" + symbolTable->size();
    
    desc_identifier idD;
    idD.identifier = nameVarTmpD;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);
    VariableOrConstante(nameVarTmpD, operandeD);

    std::string op = ctx->OP->getText();
    if(op == "*") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::mul, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else {
        //TODO : division
    }
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    string expr_finale = visit(ctx->expr());
    cfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {expr_finale});

    return 0;
}

void CodeGenVisitor::VariableOrConstante(string name1, string name2) {
    // pour gérer si on a une constante ou une variable à droite
    if (symbolTable->getIndex(name2) == -1){
        cfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {name1, name2});
    }
    else{
        cfg->current_bb->add_IRInstr(IRInstr::Operation::copy, INT, {name1, name2});
    }
}