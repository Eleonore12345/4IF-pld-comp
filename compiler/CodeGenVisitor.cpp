#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;
    std::cout<< " pushq %rbp # save %rbp on the stack" ;
    std::cout<< " movq %rsp, %rbp # define %rbp for the current function" ;

    this->visit( ctx->return_stmt() );
    std::cout<< " popq %rbp # restore %rbp from the stack" ;
    std::cout<< " ret" ;

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}
