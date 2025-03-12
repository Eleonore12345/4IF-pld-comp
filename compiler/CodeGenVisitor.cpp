#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;

    std::cout << "    # prologue\n" << "    pushq %rbp\n" << "    movq %rsp, %rbp\n" << "\n" ;

    std::cout << "    # body\n" ;
    this->visit( ctx->expr() );
    
    std::cout << "\n" << "    # epilogue\n" << "    popq %rbp\n" ;
    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDec(ifccParser::DecContext *ctx) {
    std::cout << "c"<< std::endl;
}
antlrcpp::Any CodeGenVisitor::visitAffDecConst(ifccParser::AffDecConstContext *ctx) {
    std::cout << "d" << std::endl;
}
antlrcpp::Any CodeGenVisitor::visitAffDecVar(ifccParser::AffDecVarContext *ctx) {
    std::cout << "e" << std::endl;
}
antlrcpp::Any CodeGenVisitor::visitAffVar(ifccParser::AffVarContext *ctx) {
    std::cout <<"f" << std::endl;    
}
antlrcpp::Any CodeGenVisitor::visitAffConst(ifccParser::AffConstContext *ctx) {
    std::cout << "g" << std::endl;
}