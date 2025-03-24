#include "AssemblyX86.h"

#include <vector>
#include <string>
#include <iostream>

void AssemblyX86::generateAssemblyX86(){
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;

    std::cout << "    # prologue\n" << "    pushq %rbp\n" << "    movq %rsp, %rbp\n" << "\n" ;

    std::cout << "    # body\n" ;
    vector<BasicBlock*> bbs = cfgX86->get_bbs();
    for (BasicBlock* bb : bbs){
        for (IRInstr* instr : bb->instrs){
            IRInstr::Operation op = instr->getOperation();
            vector<string> params = instr->getParams();
            //switch pour chaque opération
            switch (op){
                case 0: //ldconst

                    //gestion des char
                    int val;
                    if (params[1][0] == '\'')
                        val = (int) params[1][1];
                    else 
                        val = stoi(params[1]);

                    std::cout << "    movl $" << val << ", -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case 1: //copy
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case 2: //add
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    addl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case 3: //sub
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    subl -" << symbolTable->getOffset(params[2]) << "(%rbp), %eax\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case 4: //mul
                    std::cout << "    movl -" << symbolTable->getOffset(params[1]) << "(%rbp), %eax\n";
                    std::cout << "    imull -" << symbolTable->getOffset(params[2]) << "(%rbp)\n";
                    std::cout << "    movl %eax, -" << symbolTable->getOffset(params[0]) << "(%rbp)\n";
                    break;
                case 5: //retour
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
    
    std::cout << "\n" << "    # epilogue\n" << "    popq %rbp\n" ;
    std::cout << "    ret\n";
}