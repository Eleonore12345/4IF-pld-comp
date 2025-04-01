#include "AssemblyArm64.h"

#include <vector>
#include <string>
#include <iostream>

void AssemblyArm64::generateAssemblyArm64()
{
    std::cout << ".global _main\n";
    vector<BasicBlock *> bbs = cfgArm64->get_bbs();
    for (BasicBlock *bb : bbs)
    {
        for (IRInstr *instr : bb->instrs)
        {
            IRInstr::Operation op = instr->getOperation();
            vector<string> params = instr->getParams();
            switch (op)
            {
            case IRInstr::ldconst:
                int val;
                if (params[1][0] == '\'')
                    val = (int)params[1][1];
                else
                    val = stol(params[1]);
                std::cout << "    mov w8, #" << val << "\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::ldconstneg:
                int valNeg;
                if (params[1][0] == '\'')
                    valNeg = (int)params[1][1];
                else
                    valNeg = stol(params[1]);
                std::cout << "    mov w8, #-" << valNeg << "\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::copy:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::negexpr:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    neg w8, w8\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::add:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w1, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    add w8, w8, w1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::sub:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w1, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    sub w8, w8, w1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::mul:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w1, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    mul w8, w8, w1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::div:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w1, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    sdiv w8, w8, w1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::mod:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w1, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    sdiv w2, w8, w1\n";
                std::cout << "    mul w2, w2, w1\n";
                std::cout << "    sub w8, w8, w2\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::and_bit:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    and w8, w8, w9\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;
            case IRInstr::or_bit:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    orr w8, w8, w9\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;
            case IRInstr::xor_bit:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    eor w8, w8, w9\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;
            case IRInstr::inf:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    subs w8, w8, w9\n";
                std::cout << "    cset w8, lt\n";
                std::cout << "    and w8, w8, #0x1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;
            case IRInstr::sup:
                std::cout << "    ldr w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    subs w8, w8, w9\n";
                std::cout << "    cset w8, gt\n";
                std::cout << "    and w8, w8, #0x1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;
            case IRInstr::eq:
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    subs w8, w8, w9\n";
                std::cout << "    cset w8, eq\n";
                std::cout << "    and w8, w8, #0x1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;
            case IRInstr::diff:
                std::cout << "    ldr w9, [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    subs w8, w8, w9\n";
                std::cout << "    cset w8, ne\n";
                std::cout << "    and w8, w8, #0x1\n";
                std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                break;

            case IRInstr::retour:
            {
                if (!params.empty())
                {
                    if (symbolTable->getIndex(params[0]) != -1)
                    {
                        std::cout << "    ldr w0, [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                    }
                    else
                    {
                        int val;
                        if (params[0][0] == '\'')
                            val = (int)params[0][1];
                        else
                            val = stol(params[0]);
                        std::cout << "    mov w0, #" << val << "\n";
                    }
                }

                int nbVariablesInScope = symbolTable->getNbVariablesInScope();
                int sizeSub = (nbVariablesInScope + 2) * 4; // +2 for frame pointer and return address
                while (sizeSub % 16 != 0)
                {
                    sizeSub += 4;
                }
                std::cout << "    # epilogue\n";
                std::cout << "    ldp x29, x30, [sp, #" << sizeSub - 16 << "]\n"; // restore the frame pointer and return address
                std::cout << "    add sp, sp, #" << sizeSub << "\n";              // restore the stack pointer
                std::cout << "    ret\n";
                symbolTable->leaveScope();
                break;
            }
            case IRInstr::functionCall:
            {
                vector<string> registers = {"w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8"};
                for (int i = 3; i < params.size(); i++)
                {
                    if (symbolTable->getOffset(params[i]) == -1) // if the variable is not in the symbol table, it is a constant
                    {
                        std::cout << "    mov " << registers[i] << ", #" << params[i] << "\n";
                    }
                    else // if the variable is in the symbol table
                    {
                        std::cout << "    ldr " << registers[i] << ", [sp, #" << symbolTable->getOffset(params[i]) << "]\n";
                    }
                }
                std::cout << "    bl _" << params[1] << "\n";
                if (params[0] != "void") // if the function is not void, we store the result in the symbol table
                {
                    std::cout << "    str w8, [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                }
                break;
            }

            case IRInstr::functionDef:
            {
                symbolTable->enterScope(params[0]);
                int nbVariablesInScope = symbolTable->getNbVariablesInScope();
                int sizeSub = (nbVariablesInScope + 2) * 4; // +2 for frame pointer and return address
                while (sizeSub % 16 != 0)
                {
                    sizeSub += 4;
                }
                std::cout << "_" << params[0] << ":\n";
                std::cout << "    sub sp, sp, #" << sizeSub << "\n";
                // prologue
                std::cout << "    # prologue\n";
                std::cout << "    stp x29, x30, [sp, #" << sizeSub - 16 << "]\n";
                std::cout << "    add x29, sp, #" << sizeSub - 16 << "\n";
                // body
                vector<string> registers = {"w1", "w2", "w3", "w4", "w5", "w6", "w7", "w8"};
                std::cout << "    # body\n";
                // store parameters in the stack
                for (int i = 1; i < params.size(); i++)
                {
                    std::cout << "    str " << registers[i - 1] << ", [x29, #" << symbolTable->getOffset(params[i]) << "]\n";
                }
                break;
            }
            }
        }
    }
}