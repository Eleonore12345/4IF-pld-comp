#include "AssemblyX86.h"

#include <vector>
#include <string>
#include <iostream>

AssemblyX86::AssemblyX86(vector<CFG*> c, SymbolTable* s){
    this->cfgsX86 = c;
    this->symbolTable = s;
    s->resetAndRootToCurrent();
}


void AssemblyX86::generateAssemblyX86()
{
    std::cout << ".globl main\n";
    //Parcours de chaque CFG (1 par fonction)
    for (CFG* c : cfgsX86) {
        vector<BasicBlock *> bbs = c->get_bbs();
        //Parcours des blocs au sein du CFG
        for (BasicBlock *bb : bbs)
        {
            std::cout << bb->label << ":" << std::endl;
            //Parcours de chaque instruction du bloc avec un switch et génération du code associé
            for (IRInstr *instr : bb->instrs)
            {
                IRInstr::Operation op = instr->getOperation();
                vector<string> params = instr->getParams();
                switch (op){
                    case IRInstr::ldconst :
                    {   
                        //gestion des char (conversion en int si c'est le cas) et des int lors d'une affectation de constante 
                        int val;
                        if (params[1][0] == '\'')
                            val = (int) params[1][1];
                        else {
                            val = stol(params[1]);
                        }

                        int offset = symbolTable->getVariable(params[0])->offset;
                        std::cout << "    movl $" << val << ", " << offset << "(%rbp)\n";
                        break;
                    }
                    //gestion du cas de la négation unitaire
                    case IRInstr::ldconstneg :
                    {
                        int valNeg;
                        if (params[1][0] == '\'')
                            valNeg = (int) params[1][1];
                        else 
                            valNeg = stol(params[1]);
                        int offset = symbolTable->getVariable(params[0])->offset;
                        std::cout << "    movl $-" << valNeg << ", " << offset << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::copy :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::negexpr :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    negl	%eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::notconst :
                    {
                        int valNotConst;
                        if (params[1][0] == '\'')
                            valNotConst = (int) params[1][1];
                        else 
                            valNotConst = stol(params[1]);
                        int offset = symbolTable->getVariable(params[0])->offset;
                        std::cout << "    xor %ecx, %ecx \n";
                        std::cout << "    cmpl $" << valNotConst << ", %ecx\n";
                        std::cout << "    sete %al\n"; 
                        std::cout << "    movzbl %al, %eax\n"; 
                        std::cout << "    movl %eax, " << offset << "(%rbp)\n"; 
                        break;
                    }
                    case IRInstr::notexpr :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        std::cout << "    cmpl  $0, " << offset_1 << "(%rbp)\n"; 
                        std::cout << "    sete %al\n"; 
                        std::cout << "    movzbl %al, %eax\n"; 
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n"; 
                        break;
                    }
                    case IRInstr::add : 
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    addl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::sub : 
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    subl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::mul : 
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    imull " << offset_2 << "(%rbp)\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::div :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_2 << "(%rbp), %ecx\n";
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    cdq" << std::endl;
                        std::cout << "    idivl %ecx\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::mod :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_2 << "(%rbp), %ecx\n";
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    cdq" << std::endl;
                        std::cout << "    idivl %ecx\n";
                        std::cout << "    movl %edx, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::and_bit:
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    andl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::or_bit:
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    orl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::xor_bit:
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    xorl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::inf :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    cmpl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    setl %al\n";
                        std::cout << "    movzbl %al, %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::sup :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    cmpl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    setg %al\n";
                        std::cout << "    movzbl %al, %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::eq :  
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    cmpl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    sete %al\n";
                        std::cout << "    movzbl %al, %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::diff :
                    {
                        int offset_0 = symbolTable->getVariable(params[0])->offset;
                        int offset_1 = symbolTable->getVariable(params[1])->offset;
                        int offset_2 = symbolTable->getVariable(params[2])->offset;
                        std::cout << "    movl " << offset_2 << "(%rbp), %eax\n";
                        std::cout << "    cmpl " << offset_1 << "(%rbp), %eax\n";
                        std::cout << "    setne %al\n";
                        std::cout << "    movzbl %al, %eax\n";
                        std::cout << "    movl %eax, " << offset_0 << "(%rbp)\n";
                        break;
                    }
                    case IRInstr::retour :
                    {
                        if(!params.empty()) {
                            variable* var = symbolTable->getVariable(params[0]);
                            if (var){
                                std::cout << "    movl " << var->offset << "(%rbp), %eax\n";
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
                        // les 6 premiers arguments d'une fonction sont stockés dans les registers dédiés
                        vector<string> registers = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
                        for (int i = 3; i < 9 && i < params.size() ; i++) {
                            variable* var = symbolTable->getVariable(params[i]);
                            if(!var) {
                                std::cout << "    movl $" << params[i] << ", %" << registers[i-3] << "\n";
                            } else {
                                std::cout << "    movl " << var->offset << "(%rbp), %" << registers[i-3] << "\n";
                            }
                        }
                        // si on a plus de 6 arguments on ajoute les restants sur la pile
                        for (int i = params.size()-1; i > 8; --i) {
                            variable* var = symbolTable->getVariable(params[i]);
                            if(!var) {
                                std::cout << "    pushq $" << params[i] << "\n";
                            } else {
                                std::cout << "    movl " << var->offset << "(%rbp), %eax\n";
                                std::cout << "    pushq %rax\n";
                            }
                        }
                        std::cout << "    call " << "_" << params[2] << "\n";
                        for (int i = params.size()-1; i > 8; --i) {
                            std::cout << "    popq %rcx\n";
                        }
                        if(params[0] != "void") {
                            int offset = symbolTable->getVariable(params[1])->offset;
                            std::cout << "    movl %eax, " << offset << "(%rbp)\n";
                        }
                        break;
                    }
                    case IRInstr::functionDef:
                    {
                        FunctionScopeNode* functionScopeNode = (FunctionScopeNode*) symbolTable->getCurrentScope();
                        int sizeSub = functionScopeNode->getSize() + 4;
                        while(sizeSub % 16 != 0) {
                            sizeSub += 4;
                        }
                        std::cout << "    # prologue\n"
                            << "    pushq %rbp\n"
                            << "    movq %rsp, %rbp\n"
                            << "    subq $" << sizeSub << ", %rsp\n"
                            << "\n";
                        std::cout << "    # body\n";
                        //On transfère les 6 premiers paramètres (qui sont dans les registres) dans nos variables locales
                        vector<string> registers = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
                        for (int i = 1; i < 7 && i < params.size() ; i++) {
                            int offset = symbolTable->getVariable(params[i])->offset;
                            std::cout << "    movl %" << registers[i-1] <<", " << offset << "(%rbp)\n";
                        }
                        break;
                    }
                    case IRInstr::enter_bloc:
                    {
                        symbolTable->enterNextScope();
                        break;
                    }
                    case IRInstr::leave_bloc:
                    {
                        symbolTable->getCurrentScope()->setVisited();
                        symbolTable->leaveScope();
                        break;
                    }
                }
            }
            //si le bloc n'a aucun bloc suivant alors c'est un bloc final
            if (bb->exit_true) {
                // si un bloc a deux blocs successeurs alors on est dans le cas d'une condition (if ou while)
                // si il n'a pas de bloc suivant exit_false alors on va simplement faire un saut vers son suivant (exit_true)
                if (bb->exit_false) {
                    string label_bb_true = bb->exit_true->label;
                    string label_bb_false = bb->exit_false->label;
                    //on teste donc true ou false pour déterminer le jump à effectuer
                    std::cout << "    cmpl $0, %eax\n";
                    std::cout << "    jne " << label_bb_true << endl;
                    std::cout << "    jmp " << label_bb_false << endl;
                } else {
                    string label_bb_endif = bb->exit_true->label;
                    std::cout << "    jmp " << label_bb_endif << endl;
                }
            }
        }
        // on quitte le scope de la fonction et on set le scode à "visited"
        symbolTable->getCurrentScope()->setVisited();
        symbolTable->leaveScope();
    }
}