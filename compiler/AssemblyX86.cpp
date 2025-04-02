#include "AssemblyX86.h"

#include <vector>
#include <string>
#include <iostream>

AssemblyX86::AssemblyX86(CFG* c, SymbolTable* s){
    cfgX86 = c;
    symbolTable = s;
    s->resetAndRootToCurrent();
}

void AssemblyX86::generateAssemblyX86()
{
    std::cout << ".globl main\n";
    vector<BasicBlock *> bbs = cfgX86->get_bbs();
    for (BasicBlock *bb : bbs)
    {
        for (IRInstr *instr : bb->instrs)
        {
            IRInstr::Operation op = instr->getOperation();
            vector<string> params = instr->getParams();
            switch (op){
                case IRInstr::ldconst :
                {
                    int val;
                    if (params[1][0] == '\'')
                        val = (int) params[1][1];
                    else 
                        val = stol(params[1]);
                    int offset = symbolTable->getVariable(params[0])->offset;
                    std::cout << "    movl $" << val << ", -" << offset << "(%rbp)\n";
                    break;
                }
                case IRInstr::ldconstneg :
                {
                    int valNeg;
                    if (params[1][0] == '\'')
                        valNeg = (int) params[1][1];
                    else 
                        valNeg = stol(params[1]);
                    int offset = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    std::cout << "    movl $-" << valNeg << ", -" << offset << "(%rbp)\n";
                    break;
                }
                case IRInstr::copy :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::negexpr :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    negl	%eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::add : 
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    addl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::sub : 
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    subl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::mul : 
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    imull -" << offset_2 << "(%rbp)\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::div :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_2 << "(%rbp), %ecx\n";
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    cdq" << std::endl;
                    std::cout << "    idivl %ecx\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::mod :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_2 << "(%rbp), %ecx\n";
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    cdq" << std::endl;
                    std::cout << "    idivl %ecx\n";
                    std::cout << "    movl %edx, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::and_bit:
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    andl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::or_bit:
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    orl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::xor_bit:
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    xorl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::inf :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    setl %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::sup :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    setg %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::eq :  
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    sete %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::diff :
                {
                    int offset_0 = symbolTable->getCurrentScope()->getVariable(params[0])->offset;
                    int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                    int offset_2 = symbolTable->getCurrentScope()->getVariable(params[2])->offset;
                    std::cout << "    movl -" << offset_2 << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << offset_1 << "(%rbp), %eax\n";
                    std::cout << "    setne %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << offset_0 << "(%rbp)\n";
                    break;
                }
                case IRInstr::retour :
                {
                    if(!params.empty()) {
                        variable* var = symbolTable->getVariable(params[0]);
                        if (var){
                            std::cout << "    movl -" << var->offset << "(%rbp), %eax\n";
                        }
                        else{
                            int val;
                            if (params[0][0] == '\'')
                                val = (int) params[0][1];
                            else 
                                val = stol(params[0]);
                                std::cout << "    movl $" << val << ", %eax\n";
                        }
                    }
                    std::cout << "\n" << "    # epilogue\n" << "    movq %rbp, %rsp\n" << "    popq %rbp\n";
                    std::cout << "    ret\n";
                    break;
                }
                case IRInstr::functionCall :
                {
                    vector<string> registers = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
                    for (int i = 3; i < params.size() ; i++) {
                        variable* var = symbolTable->getCurrentScope()->getVariable(params[i]);
                        if(!var) {
                            std::cout << "    movl $" << params[i] << ", %" << registers[i-3] << "\n";
                        } else {
                            std::cout << "    movl -" << var->offset << "(%rbp), %" << registers[i-3] << "\n";
                        }
                    }
                    std::cout << "    call " << params[2] << "\n";
                    if(params[0] != "void") {
                        int offset_1 = symbolTable->getCurrentScope()->getVariable(params[1])->offset;
                        std::cout << "    movl %eax, -" << offset_1 << "(%rbp)\n";
                    }
                    break;
                }
                case IRInstr::functionDef :
                {
                    FunctionScopeNode* functionScopeNode = (FunctionScopeNode*) symbolTable->getCurrentScope();
                    int sizeSub = functionScopeNode->getSize() + 4;
                    while(sizeSub % 16 != 0) {
                        sizeSub += 4;
                    }
                    std::cout << params[0] << ":" << std::endl;
                    std::cout << "    # prologue\n"
                        << "    pushq %rbp\n"
                        << "    movq %rsp, %rbp\n"
                        << "    subq $" << sizeSub << ", %rsp\n"
                        << "\n";
                    std::cout << "    # body\n";
                    vector<string> registers = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
                    for (int i = 1; i < params.size() ; i++) {
                        int offset = symbolTable->getCurrentScope()->getVariable(params[i])->offset;
                        std::cout << "    movl %" << registers[i-1] <<", -" << offset << "(%rbp)\n";
                    }
                    break;
                }
                case IRInstr::enter_bloc :
                {
                    symbolTable->enterNextScope();
                    break;
                }
                case IRInstr::leave_bloc :
                {
                    symbolTable->getCurrentScope()->setVisited();
                    symbolTable->leaveScope();
                    break;
                }
                default :
                    break;
            }
        }
    }
}