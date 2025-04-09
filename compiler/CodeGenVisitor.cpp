#include "CodeGenVisitor.h"
#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(SymbolTable* symboleTable, FunctionTable * functionTable) : ifccBaseVisitor()
{
    symbolTable = symboleTable;
    symbolTable->resetAndRootToCurrent();
    funcTable = functionTable;
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    

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

    return ctx->VAR()->getText();
}

antlrcpp::Any CodeGenVisitor::visitVariableSimple(ifccParser::VariableSimpleContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    return varName;
}

antlrcpp::Any CodeGenVisitor::visitConstante(ifccParser::ConstanteContext *ctx) {
    std::string constant = ctx->CONST()->getText();
    return constant;
}

antlrcpp::Any CodeGenVisitor::visitOpAddSub(ifccParser::OpAddSubContext *ctx) {
    string operandeG = visit(ctx->expr(0));
    variable* varTmpG = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpG->use = true;
    string nameVarTmpG = varTmpG->name;
    VariableOrConstante(nameVarTmpG, operandeG);
    
    string operandeD = visit(ctx->expr(1));
    variable* varTmpD = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpD->use = true;
    string nameVarTmpD = varTmpD->name;
    VariableOrConstante(nameVarTmpD, operandeD);

    std::string op = ctx->OP->getText();
    if(op == "+") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::add, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::sub, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    }
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpMultDiv(ifccParser::OpMultDivContext *ctx) {
    string operandeG = visit(ctx->expr(0));
    variable* varTmpG = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpG->use = true;
    string nameVarTmpG = varTmpG->name;
    VariableOrConstante(nameVarTmpG, operandeG);
    
    string operandeD = visit(ctx->expr(1));
    variable* varTmpD = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpD->use = true;
    string nameVarTmpD = varTmpD->name;
    VariableOrConstante(nameVarTmpD, operandeD);

    std::string op = ctx->OP->getText();
    if(op == "*") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::mul, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else if(op == "/") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::div, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    } else {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::mod, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    }
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpUnConst(ifccParser::OpUnConstContext *ctx) {
    std::string opName = ctx->OP->getText();
    std::string constant = ctx->CONST()->getText();
    variable* varTmp = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmp->use = true;
    string nameVarTmp = varTmp->name;
    if (opName == "-") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::ldconstneg, INT, {nameVarTmp, constant});
    } else if (opName == "+") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {nameVarTmp, constant});
    } else if (opName == "!") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::notconst, INT, {nameVarTmp, constant});
    }
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitOpUnExpr(ifccParser::OpUnExprContext *ctx) {
    std::string opName = ctx->OP->getText();
    string operande = visit(ctx->expr());
    variable* varTmp = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmp->use = true;
    string nameVarTmp = varTmp->name;
    if (opName == "-") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::negexpr, INT, {nameVarTmp, operande});
    } else if (opName == "+") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {nameVarTmp, operande});
    } else if (opName == "!") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::notexpr, INT, {nameVarTmp, operande});
    }
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{   
    if(ctx->expr()) {
        string expr_finale = visit(ctx->expr());
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {expr_finale});
    } else {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {});
    }
    return 0;
}

void CodeGenVisitor::VariableOrConstante(string name1, string name2) {
    // pour gérer si on a une constante ou une variable à droite
    if (!symbolTable->getVariable(name2)){
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::ldconst, INT, {name1, name2});
    }
    else{
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::copy, INT, {name1, name2});
    }
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    variable* varTmpG = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpG->use = true;
    string nameVarTmpG = varTmpG->name;
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    variable* varTmpD = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpD->use = true;
    string nameVarTmpD = varTmpD->name;
    VariableOrConstante(nameVarTmpD, operandeD);

    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::and_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    variable* varTmpG = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpG->use = true;
    string nameVarTmpG = varTmpG->name;
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    variable* varTmpD = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpD->use = true;
    string nameVarTmpD = varTmpD->name;
    VariableOrConstante(nameVarTmpD, operandeD);

    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::xor_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx)
{
    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    variable* varTmpG = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpG->use = true;
    string nameVarTmpG = varTmpG->name;
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    variable* varTmpD = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpD->use = true;
    string nameVarTmpD = varTmpD->name;
    VariableOrConstante(nameVarTmpD, operandeD);

    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::or_bit, INT, {nameVarTmpG, nameVarTmpG, nameVarTmpD});
    return nameVarTmpG;
}

antlrcpp::Any CodeGenVisitor::visitOpComp(ifccParser::OpCompContext *ctx) {
    std::string opName = ctx->OP->getText();

    // Visit the left operand and store the result
    string operandeG = visit(ctx->expr(0));
    variable* varTmpG = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpG->use = true;
    string nameVarTmpG = varTmpG->name;
    VariableOrConstante(nameVarTmpG, operandeG);

    // Visit the right operand and store the result
    string operandeD = visit(ctx->expr(1));
    variable* varTmpD = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmpD->use = true;
    string nameVarTmpD = varTmpD->name;
    VariableOrConstante(nameVarTmpD, operandeD);

    variable* varTmp = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmp->use = true;
    string nameVarTmp = varTmp->name;

    if (opName == "==") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::eq, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    } else if (opName == "!=") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::diff, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    } else if (opName == "<") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::inf, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    } else if (opName == ">") {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::sup, INT, {nameVarTmp, nameVarTmpG, nameVarTmpD});
    }
    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitExpression(ifccParser::ExpressionContext *ctx)
{
    return visit(ctx->expr());
}

antlrcpp::Any CodeGenVisitor::visitDefFunc(ifccParser::DefFuncContext * ctx) {
    std::string fctName = ctx->VAR()->getText();

    //création d'un cfg par fonction
    //ajout du premier bloc de base dans le cfg
    CFG * c = new CFG();
    currentCfg = c;
    cfgs.push_back(c);
    BasicBlock* bb = new BasicBlock(currentCfg, fctName);
    currentCfg->add_bb(bb);
    currentCfg->current_bb = bb;

    std::string returnType = ctx->typeFunc()->getText();
    vector<string> paramNames = visit(ctx->params());

    funcTable->setCurrentFunction(fctName);

    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::enter_bloc, INT, {});
    symbolTable->enterNextScope();

    paramNames.insert(paramNames.begin(), fctName);

    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::functionDef, INT, paramNames);
    visitChildren(ctx);

    symbolTable->getCurrentScope()->setVisited();
    symbolTable->leaveScope();


    //Verification si un return est present dans la fonction sinon on ret vide
    if(!funcTable->hasReturn(fctName)) {
        currentCfg->current_bb->add_IRInstr(IRInstr::Operation::retour, INT, {});
    }
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

    variable* varTmp = symbolTable->getCurrentScope()->getNextNotUsedTempVar();
    varTmp->use = true;
    string nameVarTmp = varTmp->name;

    argNames.insert(argNames.begin(), nameVarTmp);

    argNames.insert(argNames.begin(), funcTable->getReturnType(fctName));
    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::functionCall, INT, argNames);

    return nameVarTmp;
}

antlrcpp::Any CodeGenVisitor::visitNoArg(ifccParser::NoArgContext *ctx)
{
    vector<string> argNames = {};
    return argNames;
}
                
antlrcpp::Any CodeGenVisitor::visitWithArgs(ifccParser::WithArgsContext *ctx)
{
    int size = ctx->expr().size();
    vector<string> args = {};
    for (int i = 0 ; i < size ; i++) {
        string arg = visit(ctx->expr(i));
        args.push_back(arg);
    }
    return args;
}

antlrcpp::Any CodeGenVisitor::visitNoParam(ifccParser::NoParamContext *ctx)
{
    vector<string> paramNames = {};
    return paramNames;
}
                
antlrcpp::Any CodeGenVisitor::visitWithParams(ifccParser::WithParamsContext *ctx)
{
    int size = ctx->VAR().size();
    vector<string> paramNames = {};
    for (int i = 0 ; i < size ; i++) {
        string varName = ctx->VAR(i)->getText();
        paramNames.push_back(varName);
    }
    return paramNames;
}

antlrcpp::Any CodeGenVisitor::visitBloc(ifccParser::BlocContext *ctx) {
    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::enter_bloc, INT, {});
    symbolTable->enterNextScope();
    visitChildren(ctx);
    currentCfg->current_bb->add_IRInstr(IRInstr::Operation::leave_bloc, INT, {});
    symbolTable->getCurrentScope()->setVisited();
    symbolTable->leaveScope();
    return 0;
}

void CodeGenVisitor::deleteCfgs(){
    for (CFG* cfg : cfgs){
        for (BasicBlock* bb : cfg->get_bbs()){
            delete(bb);
        }
        delete(cfg);
    }
}

antlrcpp::Any CodeGenVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx){
    string fctName = symbolTable->getCurrentScope()->getFunctionParent()->getName();
    // création des 3 blocs vides
    BasicBlock* bb_true = new BasicBlock(currentCfg, currentCfg->new_BB_name(fctName));
    BasicBlock* bb_false = new BasicBlock(currentCfg, currentCfg->new_BB_name(fctName));
    BasicBlock* bb_endif = new BasicBlock(currentCfg, currentCfg->new_BB_name(fctName));
    
    // affectation des pointeurs
    BasicBlock* previous_bb_endif = currentCfg->current_bb->exit_true;
    currentCfg->current_bb->exit_true = bb_true;
    currentCfg->current_bb->exit_false = bb_false;
    bb_true->exit_true = bb_endif;
    bb_false->exit_true = bb_endif;
    bb_endif->exit_true = previous_bb_endif;

    // visite de la condition
    visit(ctx->expr());

    // on passe dans la branche true
    currentCfg->add_bb(bb_true);
    currentCfg->current_bb = bb_true;
    visit(ctx->instr());

    // on passe dans la branche false
    currentCfg->add_bb(bb_false);
    if (ctx->else_stmt()) {
        currentCfg->current_bb = bb_false;
        visit(ctx->else_stmt());
    } 

    // on retourne dans la branche endif
    currentCfg->add_bb(bb_endif);
    currentCfg->current_bb = bb_endif;
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) {
    string fctName = symbolTable->getCurrentScope()->getFunctionParent()->getName();

    BasicBlock* bb_condition = new BasicBlock(currentCfg, currentCfg->new_BB_name(fctName));
    BasicBlock* bb_body = new BasicBlock(currentCfg, currentCfg->new_BB_name(fctName));
    BasicBlock* bb_sortie = new BasicBlock(currentCfg, currentCfg->new_BB_name(fctName));

    BasicBlock* previous_exit = currentCfg->current_bb->exit_true;

    currentCfg->current_bb->exit_true = bb_condition;

    bb_condition->exit_true = bb_body;
    bb_condition->exit_false = bb_sortie;

    bb_body->exit_true = bb_condition; 

    //On explore le bloc condition (l'expr) : deux issues soit on sort de la boucle soit on y reste
    currentCfg->add_bb(bb_condition);
    currentCfg->current_bb = bb_condition;
    visit(ctx->expr()); 
    

    //on explore le body et on revient a la condition
    currentCfg->add_bb(bb_body);
    currentCfg->current_bb = bb_body;
    visit(ctx->instr());
    

    // on sort de la boucle
    currentCfg->add_bb(bb_sortie);
    currentCfg->current_bb = bb_sortie;
    bb_sortie->exit_true = previous_exit;

    return 0;

}