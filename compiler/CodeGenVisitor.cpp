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

antlrcpp::Any CodeGenVisitor::visitParentheses(ifccParser::ParenthesesContext *ctx) {
    visitChildren(ctx);
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitInitDecla(ifccParser::InitDeclaContext * ctx) {
    if(ctx->expr()) {
        std::string varName = ctx->VAR()->getText();
        int index = symbolTable->getOffset(varName);
        visit(ctx->expr());
        std::cout << "    movl %eax, -" << index << "(%rbp)\n";
    }
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
    std::string constant = ctx->CONST()->getText();
    int val;
    if (constant[0] == '\'')
        val = (int) constant[1];
    else 
        val = stol(constant);

    std::cout << "    movl $" << val << ", %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitOpAddSub(ifccParser::OpAddSubContext *ctx) {
    visit(ctx->expr(0));
    string nameVarTmp = "tmp" + symbolTable->size();
    desc_identifier id;
    id.identifier = nameVarTmp;
    id.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(id);
    std::cout << "    movl %eax, -" << id.offset << "(%rbp)\n";

    visit(ctx->expr(1));
    std::string op = ctx->OP->getText();
    if(op == "+") {
        std::cout << "    addl -" << id.offset << "(%rbp), %eax\n";
    } else {
        std::cout << "    movl %eax, %ecx\n";
        std::cout << "    movl -" << id.offset << "(%rbp), %eax\n";
        std::cout << "    subl %ecx, %eax\n";
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitOpMultDiv(ifccParser::OpMultDivContext *ctx) {
    visit(ctx->expr(0));
    string nameVarTmp = "tmp" + symbolTable->size();
    desc_identifier id;
    id.identifier = nameVarTmp;
    id.offset = (symbolTable->size() + 1) * 4;
    symbolTable->addIdentifier(id);
    std::cout << "    movl %eax, -" << id.offset << "(%rbp)\n";

    visit(ctx->expr(1));
    std::string op = ctx->OP->getText();
    if(op == "*") {
        std::cout << "    imull -" << id.offset << "(%rbp)\n";
    } else {
        //TODO : checkez la division par 0
        std::cout << "    movl %eax, %ecx\n";
        std::cout << "    movl -" << id.offset << "(%rbp), %eax\n";
        std::cout << "    cdq" << std::endl;
        std::cout << "    idivl %ecx\n";
        if (op == "%") {
            std::cout << "    movl %edx, %eax\n";
        }
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitOpUnConst(ifccParser::OpUnConstContext *ctx) {
    std::string opName = ctx->OP->getText();
    std::string constant = ctx->CONST()->getText();

    int val;
    if (constant[0] == '\''){
        val = (int) constant[1];
    } else {
        val = stol(constant);
    }
    
    if (opName == "-") {
        std::cout << "    movl $-" << val << ", %eax\n";
    } else {
        std::cout << "    movl $" << val << ", %eax\n";
    }
        
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitOpUnExpr(ifccParser::OpUnExprContext *ctx) {
    std::string opName = ctx->OP->getText();
    visit(ctx->expr());

    if (opName == "-") {
        std::cout << "    negl	%eax\n";
    }
        
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    visit(ctx->expr());

    return 0;
}