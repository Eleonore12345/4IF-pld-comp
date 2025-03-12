#include "CodeGenVisitor.h"

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

antlrcpp::Any CodeGenVisitor::visitDefinitionC(ifccParser::DefinitionCContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    int index = symbolTable->getIndex(varName);

    std::cout << "    movl	$" << val << ", -" << index << "(%rbp)\n";

    visitChildren(ctx);

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitDefinitionV(ifccParser::DefinitionVContext * ctx) {
    std::string varName0 = ctx->VAR(0)->getText();
    std::string varName1 = ctx->VAR(1)->getText();
    int index0 = symbolTable->getIndex(varName0);
    int index1 = symbolTable->getIndex(varName1);

    std::cout << "    movl	-" << index1 << "(%rbp), %eax\n";
    std::cout << "    movl	%eax, -" << index0 << "(%rbp)\n";

    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx) {
    visitChildren(ctx);
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectationC(ifccParser::AffectationCContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    int val = stoi(ctx->CONST()->getText());
    int index = symbolTable->getIndex(varName);

    std::cout << "    movl	$" << val << ", -" << index << "(%rbp)\n";

    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAffectationV(ifccParser::AffectationVContext *ctx) {
    std::string varName0 = ctx->VAR(0)->getText();
    std::string varName1 = ctx->VAR(1)->getText();
    int index0 = symbolTable->getIndex(varName0);
    int index1 = symbolTable->getIndex(varName1);

    std::cout << "    movl	-" << index1 << "(%rbp), %eax\n";
    std::cout << "    movl	%eax, -" << index0 << "(%rbp)\n";
    
    visitChildren(ctx);

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}