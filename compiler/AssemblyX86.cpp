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
                    int val;
                    if (params[1][0] == '\'')
                        val = (int) params[1][1];
                    else 
                        val = stol(params[1]);
                    std::cout << "    movl $" << val << ", -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::ldconstneg :
                    int valNeg;
                    if (params[1][0] == '\'')
                        valNeg = (int) params[1][1];
                    else 
                        valNeg = stol(params[1]);
                    std::cout << "    movl $-" << valNeg << ", -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::copy :
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::negexpr :
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    negl	%eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::add : 
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    addl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::sub : 
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    subl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::mul : 
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    imull -" << symbolTable->getOffset(params[2]) << "(%rbp)\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::div :
                    std::cout << "    movl -" << symbolTable->getOffset(params[2]) << "(%rbp), %ecx\n";
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    cdq" << std::endl;
                    std::cout << "    idivl %ecx\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::mod :
                    std::cout << "    movl -" << symbolTable->getOffset(params[2]) << "(%rbp), %ecx\n";
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    cdq" << std::endl;
                    std::cout << "    idivl %ecx\n";
                    std::cout << "    movl %edx, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::and_bit:
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    andl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::or_bit:
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    orl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::xor_bit:
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    xorl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::inf :
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    setl %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::sup :
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    setg %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::eq :  
                    std::cout << "    movl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    sete %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::diff :
                    std::cout << "    movl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    cmpl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    setne %al\n";
                    std::cout << "    movzbl %al, %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::retour :
                    if(!params.empty()) {
                        if (symbolTable->getIndex(params[0]) != -1){
                            std::cout << "    movl -" << symbolTable->getOffset(params[0]) << "(%rbp), %eax\n";
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
                case IRInstr::functionCall :
                {
                    vector<string> registers = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
                    for (int i = 3; i < params.size() ; i++) {
                        if(symbolTable->getOffset(params[i]) == -1) {
                            std::cout << "    movl $" << params[i] << ", %" << registers[i-3] << "\n";
                        } else {
                            std::cout << "    movl -" << symbolTable->getOffset(params[i]) << "(%rbp), %" << registers[i-3] << "\n";
                        }
                    }
                    std::cout << "    call " << params[2] << "\n";
                    if(params[0] != "void") {
                        std::cout << "    movl %eax, -" << symbolTable->getOffset(params[1]) << "(%rbp)\n";
                    }
                    break;
                }
                case IRInstr::functionDef :
                {
                    int nbVariablesInScope = symbolTable->getNbVariablesInScope();
                    int sizeSub = (nbVariablesInScope+1)*4;
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
                        std::cout << "    movl %" << registers[i-1] <<", -" << symbolTable->getOffset(params[i]) << "(%rbp)\n";
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
                    symbolTable->setCurrentScopeVisited();
                    symbolTable->leaveScope();
                    break;
                }
                default :
                    break;
            }
        }
    }
}