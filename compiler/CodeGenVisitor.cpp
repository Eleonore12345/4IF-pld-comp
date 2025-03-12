#include "CodeGenVisitor.h"

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;

    std::cout << "    # prologue\n" << "    pushq %rbp\n" << "    movq %rsp, %rbp\n" << "\n" ;

    std::cout << "    # body\n" ;
    this->visit(ctx->expr());
    
    std::cout << "\n" << "    # epilogue\n" << "    popq %rbp\n" ;
    std::cout << "    ret\n";

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) {
    int val = stoi(ctx->CONST()->getText());

    std::cout << "    movl	$" << val << ", -4(%rbp)\n";

    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclarationV(ifccParser::DeclarationVContext *ctx) {
    this->visit(ctx->expr());
    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}

bool CodeGenVisitor::isInTable(std::string varName) {
    for (size_t i = 0; i < symboleTable.size(); i++) {
        if(varName.compare(symboleTable[i].name) == 0) {
            return true;
        }
    }
    return false;
}
