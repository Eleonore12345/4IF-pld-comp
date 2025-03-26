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
    symTable->createAndEnterScope(funcName);
    int nbParams = visit(ctx->params());
    if(!funcTable->isPresent(funcName)) {
        function_identifier f;
        f.functionName = funcName;
        f.nbParams = nbParams;
        f.def = true;
        funcTable->addFunction(f);
    } else {
        funcTable->setDef(funcName);
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
    for(int i = 0; i < size; i++) {
        string varName = ctx->VAR(i)->getText();
        desc_identifier id;
        id.identifier = varName;
        id.offset = (symTable->size() + 1) * 4;
        id.init = true;
        symTable->addIdentifier(id);
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
        visit(ctx->expr(i));
    }
    return size;
}