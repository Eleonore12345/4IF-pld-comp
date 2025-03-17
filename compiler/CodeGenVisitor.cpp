#include "CodeGenVisitor.h"

CodeGenVisitor::CodeGenVisitor(SymbolTable* symboleTable) : ifccBaseVisitor()
{
    symbolTable = symboleTable;
}

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;

    std::cout << "    # prologue\n" << "    pushq %rbp\n" << "    movq %rsp, %rbp\n" << "\n" ;

    std::cout << "    # body\n" ;
    visitChildren(ctx);
    
    std::cout << "\n" << "    # epilogue\n" << "    popq %rbp\n" ;
    std::cout << "    ret\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDefinition(ifccParser::DefinitionContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int index = symbolTable->getOffset(varName);
    visit(ctx->expr());

    std::cout << "    movl %eax, -" << index << "(%rbp)\n";

    visitChildren(ctx);

    return 0;
}



antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
    visitChildren(ctx);
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectation(ifccParser::AffectationContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    visit(ctx->expr());
    int index = symbolTable->getOffset(varName);
    std::cout << "    movl %eax, -" << index << "(%rbp)\n";
    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitVariableSimple(ifccParser::VariableSimpleContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int index = symbolTable->getOffset(varName);

    std::cout << "    movl	-" << index << "(%rbp), %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitConstante(ifccParser::ConstanteContext *ctx) {
    int val = stoi(ctx->CONST()->getText());

    std::cout << "    movl $" << val << ", %eax\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}