#include "IdentifierVisitor.h"
#include <iostream>

IdentifierVisitor::IdentifierVisitor(SymbolTable* symboleTable, FunctionTable * functionTable) : ifccBaseVisitor()
{
    symbolTable = symboleTable;
    funcTable = functionTable;
    //On doit avoir une définition d'un main
    function_identifier f;
    f.functionName = "main";
    f.nbParams = 0;
    f.def = false;
    funcTable->addFunction(f);
}

antlrcpp::Any IdentifierVisitor::visitInitDecla(ifccParser::InitDeclaContext *ctx) {
    string varName = ctx->VAR()->getText();
    // Vérifie si la variable est déjà déclarée
    if (symbolTable->getCurrentScope()->getVariable(varName)) {
        std::string erreur = "Variable " + varName + " already declared\n";
        throw std::runtime_error(erreur);
    } else {
        bool init = false;
        if(ctx->expr()) {
            verifExprPasFctVoid(ctx->expr());
            init = true;
        }
        // Ajout d'une variable dans le scope de la fonction courante
        ScopeNode* currentScope = symbolTable->getCurrentScope();
        FunctionScopeNode* functionParent = currentScope->getFunctionParent();
        functionParent->incrementSize(4);
        int offset = - functionParent->getSize();
        currentScope->addVariable(varName, offset, false, init, false);
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitAffectation(ifccParser::AffectationContext *ctx)
{
    //Permet de vérifier si une variable est déclarée et de la set à "initialisée"
    string varName = ctx->VAR()->getText();
    verifExprPasFctVoid(ctx->expr());
    variable* var = symbolTable->getVariable(varName);
    if (!var) {
        std::string erreur = "Variable " + varName + " not declared\n";
        throw std::runtime_error(erreur);
    }
    var->init = true;
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitVariableSimple(ifccParser::VariableSimpleContext *ctx) {
    // Permet de vérifier si une variable est déclarée et de la set à "utilisée"
    string varName = ctx->VAR()->getText();
    variable* var = symbolTable->getVariable(varName);
    if (!var) {
        std::string erreur = "Variable " + varName + " not declared\n";
        throw std::runtime_error(erreur);
    }
    else{
        if (var->init == 0) {
            cerr << "WARNING : variable " << varName << " used but not initialized" << endl;
        }
        var->use = true;
    }
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpMultDiv(ifccParser::OpMultDivContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    // Création des variables temporaires nécessaires
    addTempVariable();
    addTempVariable();
    antlrcpp::Any result = visit(ctx->expr(1));
    if (ctx->OP->getText()=="/" && result.is<bool>() && result.as<bool>()) {
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
    // Création des variables temporaires nécessaires
    addTempVariable();
    // Si fonction non définie et non présente dans la table de fonction alors on l'ajoute
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
    // Vérifications de fin de visite
    funcTable->checkIfEachFuncDefined();
    funcTable->checkRvalFuncReturnType();
    symbolTable->checkIfEachIdUsed();
    symbolTable->checkIfEachIdInit();
    return 0;    
}

antlrcpp::Any IdentifierVisitor::visitDefFunc(ifccParser::DefFuncContext * ctx) {
    // Création et Entrée dans le nouveau scope
    std::string funcName = ctx->VAR()->getText();
    std::string returnType = ctx->typeFunc()->getText();

    funcTable->setCurrentFunction(funcName);
    symbolTable->createAndEnterFunctionScope(funcTable->getCurrentFunction());

    int nbParams = visit(ctx->params());
    
    // Gestion de la table de fonction
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
    // Parcours des instructions
    for (int i = 0 ; i < ctx->instrOrDecla().size() ; i++) {
        visit(ctx->instrOrDecla(i));
    } 
    // On quitte le scope de la fonction
    symbolTable->leaveScope();
    return 0;
}

antlrcpp::Any IdentifierVisitor::visitNoParam(ifccParser::NoParamContext *ctx) {
    return 0;
}
                
antlrcpp::Any IdentifierVisitor::visitWithParams(ifccParser::WithParamsContext *ctx) {
    int size = ctx->VAR().size();
    ScopeNode* currentScope = symbolTable->getCurrentScope();
    FunctionScopeNode* functionParent = currentScope->getFunctionParent();
    // Gestion des 6 premiers paramètres dans les registres (puis dans les variables locales)
    for(int i = 0; i < 6 && i < size; i++) {
        string varName = ctx->VAR(i)->getText();
        functionParent->incrementSize(4);
        int offset = - functionParent->getSize();
        currentScope->addVariable(varName, offset, false, true, false);
    }
    // Gestion différente de l'offset des suivants
    for(int i = size - 1; i > 5; --i) {
        string varName = ctx->VAR(i)->getText();
        int offset = 16 + (i-6) * 8;
        variable* var = currentScope->addVariable(varName, offset, false, true, false);
    }
    return size;
}

antlrcpp::Any IdentifierVisitor::visitNoArg(ifccParser::NoArgContext *ctx) {
    return 0;
}
                
antlrcpp::Any IdentifierVisitor::visitWithArgs(ifccParser::WithArgsContext *ctx) {
    // Vérification et Exploration sur les arguments lors de l'appel de fonction
    int size = ctx->expr().size();
    for(int i = 0; i < size; i++) {
        verifExprPasFctVoid(ctx->expr(i));
        visit(ctx->expr(i));
    }
    return size;
}

antlrcpp::Any IdentifierVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {   
    funcTable->setHasReturnTrue(funcTable->getCurrentFunction());

    if(ctx->expr()) {
        verifExprPasFctVoid(ctx->expr());
        visit(ctx->expr());
        // si return expr avec une fonction void -> warning
        if(funcTable->getReturnType(funcTable->getCurrentFunction()) == "void") {
            std::cerr << "WARNING : return with a value, in function returning void : " << funcTable->getCurrentFunction() << std::endl;
        }
    }
    return 0;
}

void IdentifierVisitor::verifExprPasFctVoid(ifccParser::ExprContext * ctx) {
    // Vérification courante que l'appel d'une fonction void ne soit pas faite où on attend une valeur retournée
    if (auto funcCallCtx = dynamic_cast<ifccParser::FunctionCallContext*>(ctx)) {
        std::string funcName = funcCallCtx->VAR()->getText();
        std::string returnType = funcTable->getReturnType(funcName);
        // Ajout de la fonction si définition implicite
        if(!funcTable->isPresent(funcName) && !funcTable->isDefined(funcName)) {
            int nbArgs = visit(funcCallCtx->args());
            function_identifier f;
            f.functionName = funcName;
            f.nbParams = nbArgs;
            f.def = false;
            funcTable->addFunction(f);
        }
        funcTable->setAsRval(funcName);
        // Erreur si fonction avec un type de retour void
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
    // Création des variables temporaires nécessaires
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpUnExpr(ifccParser::OpUnExprContext *ctx) {
    verifExprPasFctVoid(ctx->expr());
    // Création des variables temporaires nécessaires
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    // Création des variables temporaires nécessaires
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1)); 
    // Création des variables temporaires nécessaires
    addTempVariable();
    addTempVariable();   
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    // Création des variables temporaires nécessaires
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitOpComp(ifccParser::OpCompContext *ctx) {
    verifExprPasFctVoid(ctx->expr(0));
    verifExprPasFctVoid(ctx->expr(1));
    // Création des variables temporaires nécessaires
    addTempVariable();
    addTempVariable();
    addTempVariable();
    return visitChildren(ctx);
}

void IdentifierVisitor::addTempVariable() {
    // Création d'une variable temporaire dans le scope courant
    string nameVarTmp = "tmp" + to_string(symbolTable->getCurrentScope()->getNbTmpVariable());
    ScopeNode* currentScope = symbolTable->getCurrentScope();
    FunctionScopeNode* functionParent = currentScope->getFunctionParent();
    functionParent->incrementSize(4);
    int offset = - functionParent->getSize();
    currentScope->addVariable(nameVarTmp, offset, false, false, true);
}

antlrcpp::Any IdentifierVisitor::visitOpUnConst(ifccParser::OpUnConstContext *ctx) {
    // Création des variables temporaires nécessaires
    addTempVariable();
    return visitChildren(ctx);
}

antlrcpp::Any IdentifierVisitor::visitBloc(ifccParser::BlocContext *ctx) {
    // Entrée dans un nouveau scope
    symbolTable->createAndEnterScope();
    visitChildren(ctx);
    // Sortie du scope du bloc
    symbolTable->leaveScope();
    return 0;
}