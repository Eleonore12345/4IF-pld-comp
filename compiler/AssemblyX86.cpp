#include "AssemblyX86.h"

#include <vector>
#include <string>
#include <iostream>

void AssemblyX86::generateAssemblyX86()
{
    std::cout << ".globl main\n";
    std::cout << " main: \n";

    std::cout << "    # prologue\n"
              << "    pushq %rbp\n"
              << "    movq %rsp, %rbp\n"
              << "\n";
    std::cout << "    # body\n";
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
                case IRInstr::and_bit: // and_bit
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    andl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::or_bit: // or_bit
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    orl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case IRInstr::xor_bit: // xor_bit
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
                    if (symbolTable->getIndex(params[0]) != -1){
                        std::cout << "    movl -" << symbolTable->getOffset(params[0]) << "(%rbp), %eax\n";
                    }
                    else{
                        int val;
                        if (params[0][0] == '\'')
                            val = (int) params[0][1];
                        else 
                            val = stoi(params[0]);
                            std::cout << "    movl $" << val << ", %eax\n";
                    }
                    std::cout << "\n" << "    # epilogue\n" << "    movq %rbp, %rsp\n" << "    popq %rbp\n";
                    std::cout << "    ret\n";
                    break;
                case IRInstr::functionCall:
                    {
                        vector<string> registers = {"edi", "esi", "edx", "ecx", "r8", "r9"};
                        for (int i = 2; i < params.size() ; i++) {
                            std::cout << "    movl -" << symbolTable->getOffset(params[i]) << "(%rbp), %" << registers[i-2] << "\n";
                        }
                        std::cout << "    call " << params[1] << "\n";
                        std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                        break;
                    }
                default :
                    break;
            }
        
        }
    }
}