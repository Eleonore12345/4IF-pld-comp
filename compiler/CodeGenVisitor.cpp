#include "CodeGenVisitor.h"
#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(SymbolTable* symboleTable, CFG* c, FunctionTable * functionTable) : ifccBaseVisitor()
{
    symbolTable = symboleTable;
    symbolTable->rootToCurrent();
    cfg = c;
    funcTable = functionTable;
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    //ajout du premier bloc de base dans le cfg
    BasicBlock* bb = new BasicBlock(cfg, cfg->new_BB_name());
    cfg->add_bb(bb);
    cfg->current_bb = bb;

    return visitChildren(ctx);
}

antlrcpp::Any CodeGenVisitor::visitInitDecla(ifccParser::InitDeclaContext * ctx) {
    if(ctx->expr()) {
        std::string exprResult = visit(ctx->expr());
        VariableOrConstante(ctx->VAR()->getText(),exprResult);
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitParentheses(ifccParser::ParenthesesContext *ctx) {
    return visit(ctx->expr());
}

antlrcpp::Any CodeGenVisitor::visitAffectation(ifccParser::AffectationContext *ctx) {

    string expr_content = visit(ctx->expr());
    // pour gérer si on a une constante ou une variable à droite
    VariableOrConstante(ctx->VAR()->getText(), expr_content);

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
    string nameVarTmpG = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpG);
    VariableOrConstante(nameVarTmpG, operandeG);
    
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpD);
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
    string nameVarTmpG = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpG);
    VariableOrConstante(nameVarTmpG, operandeG);
    
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpD);
    VariableOrConstante(nameVarTmpD, operandeD);

    std::string op = ctx->OP->getText();
    if(op == "*") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::mul, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else if(op == "/") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::div, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::mod, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    }
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpUnConst(ifccParser::OpUnConstContext *ctx) {
    std::string opName = ctx->OP->getText();
    std::string constant = ctx->CONST()->getText();
    string nameVarTmp = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmp);
    if (opName == "-") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::ldconstneg, INT, {nameVarTmp, constant});
    } else if (opName == "+") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {nameVarTmp, constant});
    } else if (opName == "!") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::notconst, INT, {nameVarTmp, constant});
    }  
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitOpUnExpr(ifccParser::OpUnExprContext *ctx) {
    std::string opName = ctx->OP->getText();
    string operande = visit(ctx->expr());
    string nameVarTmp = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmp);
    if (opName == "-") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::negexpr, INT, {nameVarTmp, operande});
    } else if (opName == "+") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {nameVarTmp, operande});
    } else if (opName == "!") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::notexpr, INT, {nameVarTmp, operande});
    } 
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    if(ctx->expr()) {
        string expr_finale = visit(ctx->expr());
        cfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {expr_finale});
    } else {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {});
    }
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

antlrcpp::Any CodeGenVisitor::visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpD);
    VariableOrConstante(nameVarTmpD, operandeD);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::and_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpD);
    VariableOrConstante(nameVarTmpD, operandeD);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::xor_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpD);
    VariableOrConstante(nameVarTmpD, operandeD);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::or_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpComp(ifccParser::OpCompContext *ctx) {
    std::string opName = ctx->OP->getText();

    // Evaluate left-hand side and store it in a tmp
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Evaluate the right-hand side and store it in a tmp
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmpD);
    VariableOrConstante(nameVarTmpD, operandeD);

    string nameVarTmp = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmp);

    if (opName == "==") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::eq, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    } else if (opName == "!=") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::diff, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    } else if (opName == "<") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::inf, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    } else if (opName == ">") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::sup, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    }
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitExpression(ifccParser::ExpressionContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any CodeGenVisitor::visitDefFunc(ifccParser::DefFuncContext * ctx) {
    std::string fctName = ctx->VAR()->getText();
    std::string returnType = ctx->typeFunc()->getText();
    symbolTable->enterScope(fctName);
    vector<string> paramNames = visit(ctx->params());

    paramNames.insert(paramNames.begin(), fctName);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::functionDef, INT, paramNames);
    visitChildren(ctx);

    //Verification si un return est present dans la fonction sinon on ret vide
    bool foundReturn = false;
    for(int i = 0; i < ctx->instr().size(); i++) {
        if(ctx->instr(i)->getText().rfind("return",0) == 0) {
            foundReturn = true;
        }
    }
    if(!foundReturn) {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {});
    }
    symbolTable->leaveScope();
    return 0;
}
                
antlrcpp::Any CodeGenVisitor::visitFunctionCall(ifccParser::FunctionCallContext *ctx) 
{
    std::string fctName = ctx->VAR()->getText();
    vector<string> argNames = visit(ctx->args());

    if (funcTable->isDefined(fctName) && argNames.size() != funcTable->getNbParams(fctName)) {
        std::string erreur;
        if (argNames.size() > funcTable->getNbParams(fctName)) {
            erreur =  "too many arguments to function '" + fctName + "'\n";
        } else {
            erreur =  "too few arguments to function '" + fctName + "'\n";
        }
        throw std::runtime_error(erreur);
    }
    
    argNames.insert(argNames.begin(), fctName);

    string nameVarTmp = symbolTable->getNextNotUsedTempVar();
    symbolTable->setUse(nameVarTmp);

    argNames.insert(argNames.begin(), nameVarTmp);

    argNames.insert(argNames.begin(), funcTable->getReturnType(fctName));
    cfg->current_bb->add_IRInstr(IRInstr::Operation::functionCall, INT, argNames);

    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitNoArg(ifccParser::NoArgContext *ctx)
{
    vector<string> argNames;
    return argNames;
}
                
antlrcpp::Any CodeGenVisitor::visitWithArgs(ifccParser::WithArgsContext *ctx)
{
    int size = ctx->expr().size();
    vector<string> args;
    for (int i = 0 ; i < size ; i++) {
        string arg = visit(ctx->expr(i));
        args.push_back(arg);
    }
    return args;
}

antlrcpp::Any CodeGenVisitor::visitNoParam(ifccParser::NoParamContext *ctx)
{
    vector<string> paramNames;
    return paramNames;
}
                
antlrcpp::Any CodeGenVisitor::visitWithParams(ifccParser::WithParamsContext *ctx)
{
    int size = ctx->VAR().size();
    vector<string> paramNames;
    for (int i = 0 ; i < size ; i++) {
        string varName = ctx->VAR(i)->getText();
        paramNames.push_back(varName);
    }
    return paramNames;
}