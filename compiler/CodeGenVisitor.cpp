#include "CodeGenVisitor.h"
#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(SymbolTable* symboleTable, CFG* c, FunctionTable * functionTable) : ifccBaseVisitor()
{
    symbolTable = symboleTable;
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

    return visitChildren(ctx);
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
    string nameVarTmp = "tmp" + symbolTable->size();
    desc_identifier id;
    id.identifier = nameVarTmp;
    id.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(id);

    if (opName == "-") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::ldconstneg, INT, {nameVarTmp, constant});
    } else {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {nameVarTmp, constant});
    }  
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitOpUnExpr(ifccParser::OpUnExprContext *ctx) {
    std::string opName = ctx->OP->getText();
    string operande = visit(ctx->expr());
    string nameVarTmp = "tmp" + symbolTable->size();
    desc_identifier id;
    id.identifier = nameVarTmp;
    id.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(id);

    if (opName == "-") {
        cfg->current_bb->add_IRInstr(IRInstr::Operation::negexpr, INT, {nameVarTmp, operande});
    } else {
        return operande;
    }
    return nameVarTmp;
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

antlrcpp::Any CodeGenVisitor::visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = "tmp" + to_string(symbolTable->size());
    desc_identifier idG;
    idG.identifier = nameVarTmpG;
    idG.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = "tmp" + to_string(symbolTable->size());
    desc_identifier idD;
    idD.identifier = nameVarTmpD;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);
    VariableOrConstante(nameVarTmpD, operandeD);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::and_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = "tmp" + to_string(symbolTable->size());
    desc_identifier idG;
    idG.identifier = nameVarTmpG;
    idG.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = "tmp" + to_string(symbolTable->size());
    desc_identifier idD;
    idD.identifier = nameVarTmpD;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);
    VariableOrConstante(nameVarTmpD, operandeD);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::xor_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    string nameVarTmpG = "tmp" + to_string(symbolTable->size());
    desc_identifier idG;
    idG.identifier = nameVarTmpG;
    idG.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    string nameVarTmpD = "tmp" + to_string(symbolTable->size());
    desc_identifier idD;
    idD.identifier = nameVarTmpD;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);
    VariableOrConstante(nameVarTmpD, operandeD);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::or_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpComp(ifccParser::OpCompContext *ctx) {
    std::string opName = ctx->OP->getText();

    // Evaluate left-hand side and store it in a tmp
    string operandeG = visit(ctx->expr(0));
    std::string nameVarTmpG = "tmp" + std::to_string(symbolTable->size());
    desc_identifier idG;
    idG.identifier = nameVarTmpG;
    idG.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idG);
    VariableOrConstante(nameVarTmpG, operandeG);

    // Evaluate the right-hand side and store it in a tmp
    string operandeD = visit(ctx->expr(1));
    std::string nameVarTmpD = "tmp" + std::to_string(symbolTable->size());

    desc_identifier idD;
    idD.identifier = nameVarTmpD;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);
    VariableOrConstante(nameVarTmpD, operandeD);

    std::string nameVarTmp = "tmp" + std::to_string(symbolTable->size());
    desc_identifier idR;
    idR.identifier = nameVarTmp;
    idR.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idR);

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
    vector<string> paramNames = visit(ctx->params());

    paramNames.insert(paramNames.begin(), fctName);

    cfg->current_bb->add_IRInstr(IRInstr::Operation::functionDef, INT, paramNames);

    return visitChildren(ctx);
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

    string nameVarTmp = "tmp" + symbolTable->size();
    desc_identifier idD;
    idD.identifier = nameVarTmp;
    idD.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(idD);

    argNames.insert(argNames.begin(), nameVarTmp);

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
    vector<string> argNames;
    for (int i = 0 ; i < size ; i++) {
        string varTmpName = visit(ctx->expr(i));
        argNames.push_back(varTmpName);
    }
    return argNames;
}

antlrcpp::Any CodeGenVisitor::visitNoParam(ifccParser::NoParamContext *ctx)
{
    vector<string> paramNames;
    return paramNames;
}
                
antlrcpp::Any CodeGenVisitor::visitWithParams(ifccParser::WithParamsContext *ctx)
{
    int size = ctx->expr().size();
    vector<string> paramNames;
    for (int i = 0 ; i < size ; i++) {
        string varTmpName = visit(ctx->expr(i));
        paramNames.push_back(varTmpName);
    }
    return paramNames;
}