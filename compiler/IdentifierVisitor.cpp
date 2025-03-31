#include "IdentifierVisitor.h"
#include <iostream>

IdentifierVisitor::IdentifierVisitor(SymbolTable* symboleTable, FunctionTable * functionTable) : ifccBaseVisitor()
{
    symTable = symboleTable;
    funcTable = functionTable;
    //On doit avoir une définition d'un main
    function_identifier f;
    f.functionName = "main";
    f.nbParams = 0;
    f.def = false;
    funcTable->addFunction(f);
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
            verifExprPasFctVoid(ctx->expr());
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
    verifExprPasFctVoid(ctx->expr());
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
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    addTempVariable();
    addTempVariable();
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

antlrcpp::Any IdentifierVisitor::visitFunctionCall(ifccParser::FunctionCallContext *ctx) {
    std::string funcName = ctx->VAR()->getText();
    addTempVariable();
    if(!funcTable->isDefined(funcName)) {
        std::cerr << "WARNING : implicit declaration of function " << funcName << std::endl;
        if(!funcTable->isPresent(funcName)) {
            int nbArgs = visit(ctx->args());
            function_identifier f;
            f.functionName = funcName;
            f.nbParams = nbArgs;
            f.def = false;
            funcTable->addFunction(f);
        }
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAxiom(ifccParser::AxiomContext *ctx)
{
    visitChildren(ctx);
    funcTable->checkIfEachFuncDefined();
    symTable->checkIfEachIdUsed();
    symTable->checkIfEachIdInit();
    return 0;    
}

antlrcpp::Any IdentifierVisitor::visitDefFunc(ifccParser::DefFuncContext * ctx) {
    std::string funcName = ctx->VAR()->getText();
    std::string returnType = ctx->typeFunc()->getText();
    symTable->createAndEnterScope(funcName);
    funcTable->setCurrentFunction(funcName);
    int nbParams = visit(ctx->params());
    if(!funcTable->isPresent(funcName)) {
        function_identifier f;
        f.functionName = funcName;
        f.retourType = returnType;
        f.nbParams = nbParams;
        f.def = true;
        funcTable->addFunction(f);
    } else {
        if(funcTable->isDefined(funcName)) {
            std::string erreur = "Function " + funcName + " already defined\n";
            throw std::runtime_error(erreur);
        } else {
            funcTable->setDef(funcName);
            funcTable->setReturnType(funcName,returnType);
        }
    }
    for(int i = 0; i < ctx->instr().size(); i++) {
        visit(ctx->instr(i));
    }
    symTable->leaveScope();
    return 0;
}

antlrcpp::Any IdentifierVisitor::visitNoParam(ifccParser::NoParamContext *ctx)
{
    return 0;
}
                
antlrcpp::Any IdentifierVisitor::visitWithParams(ifccParser::WithParamsContext *ctx)
{
    int size = ctx->VAR().size();
    if(size > 6) {
        std::string erreur = "Fonctions avec plus de 6 paramètres non implémentées\n";
        throw std::runtime_error(erreur);
    } else {
        for(int i = 0; i < size; i++) {
            string varName = ctx->VAR(i)->getText();
            desc_identifier id;
            id.identifier = varName;
            id.offset = (symTable->size() + 1) * 4;
            id.init = true;
            symTable->addIdentifier(id);
        }
    }
    return size;
}

antlrcpp::Any IdentifierVisitor::visitNoArg(ifccParser::NoArgContext *ctx)
{
    return 0;
}
                
antlrcpp::Any IdentifierVisitor::visitWithArgs(ifccParser::WithArgsContext *ctx)
{
    int size = ctx->expr().size();
    for(int i = 0; i < size; i++) {
        verifExprPasFctVoid(ctx->expr(i));
        visit(ctx->expr(i));
    }
    return size;
}

antlrcpp::Any IdentifierVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {   
    if(ctx->expr()) {
        verifExprPasFctVoid(ctx->expr());
        visit(ctx->expr());
        if(funcTable->getReturnType(funcTable->getCurrentFunction()) == "void") {
            std::cerr << "WARNING : return with a value, in function returning void : " << funcTable->getCurrentFunction() << std::endl;
        }
    }
    return 0;
}

void IdentifierVisitor::verifExprPasFctVoid(ifccParser::ExprContext * ctx) {
    if (auto funcCallCtx = dynamic_cast<ifccParser::FunctionCallContext*>(ctx)) {
        std::string funcName = funcCallCtx->VAR()->getText();
        std::string returnType = funcTable->getReturnType(funcName);
        if (returnType == "void") {
            std::string erreur = "error: void value not ignored as it ought to be\n";
            throw std::runtime_error(erreur);
        }
    }
}

antlrcpp::Any IdentifierVisitor::visitParentheses(ifccParser::ParenthesesContext *ctx) {
    verifExprPasFctVoid(ctx->expr());
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpAddSub(ifccParser::OpAddSubContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpUnExpr(ifccParser::OpUnExprContext *ctx) {
    verifExprPasFctVoid(ctx->expr());
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1)); 
    addTempVariable();
    addTempVariable();   
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpComp(ifccParser::OpCompContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    addTempVariable();
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

void IdentifierVisitor::addTempVariable() {
    string nameVarTmp = "tmp" + to_string(symTable->size());
    desc_identifier id;
    id.identifier = nameVarTmp;
    id.isTemp = true;
    id.offset = (symTable->size() + 1) * 4;
    symTable->addIdentifier(id);
}

antlrcpp::Any IdentifierVisitor::visitOpUnConst(ifccParser::OpUnConstContext *ctx) {
    addTempVariable();
    return visitChildren(ctx);
}