#include "CodeGenVisitor.h"
#include "CodeGenSymbolTable.h"

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


antlrcpp::Any CodeGenVisitor::visitDeclarationV(ifccParser::DeclarationVContext *ctx)
{
    visitChildren(ctx);
    return 0;
}
/*
antlrcpp::Any CodeGenVisitor::visitDeclarationVaV(ifccParser::DeclarationVaVContext *ctx)
{

}
*/

antlrcpp::Any CodeGenVisitor::visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    int value = stoi(ctx->CONST()->getText());
    int index = getIndex(varName);

    std::cout << "    movl $" << value << ", -" << index <<"(%rbp)\n";  

    visitChildren(ctx);

    return 0;
}
/*
antlrcpp::Any CodeGenVisitor::visitAffectationVaV(ifccParser::AffectationVaVContext *ctx)
{

}

antlrcpp::Any CodeGenVisitor::visitAffectationVaC(ifccParser::AffectationVacContext *ctx)
{

}
*/
antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}

int CodeGenVisitor::getIndex(std::string varName)
{
    for (size_t i = 0; i < symbolTable.size(); i++) {
        if(varName.compare(symbolTable[i].name) == 0) {
            return symbolTable[i].index;
        }
    }
    return 0;
}
