#include "AssemblyArm64.h"
#include <vector>
#include <string>
#include <iostream>
#include <stack>

// Implémenter un algo de gestion des registres pour éviter les conflits d'affectation simultanée
class RegisterAllocator
{
    std::stack<std::string> freeRegisters;

public:
    RegisterAllocator()
    {
        for (int i = 8; i <= 15; ++i)
        {
            freeRegisters.push("w" + std::to_string(i));
        }
    }

    std::string allocate()
    {
        if (freeRegisters.empty())
        {
            // Pas de registre disponible, utiliser le stack
            return "spill";
        }
        std::string reg = freeRegisters.top();
        freeRegisters.pop();
        return reg;
    }

    void free(const std::string &reg)
    {
        if (reg != "spill")
        {
            freeRegisters.push(reg);
        }
    }
};

void AssemblyArm64::generateAssemblyArm64()
{
    std::cout << ".global _main\n";
    RegisterAllocator regAlloc;

    vector<BasicBlock *> bbs = cfgArm64->get_bbs();
    for (BasicBlock *bb : bbs)
    {
        if (!bb->label.empty())
        {
            std::cout << bb->label << ":\n";
        }

        for (IRInstr *instr : bb->instrs)
        {
            IRInstr::Operation op = instr->getOperation();
            vector<string> params = instr->getParams();

            switch (op)
            {
            case IRInstr::ldconst:
            {
                std::string reg = regAlloc.allocate();
                int val = (params[1][0] == '\'') ? (int)params[1][1] : stol(params[1]);
                std::cout << "    mov " << reg << ", #" << val << "\n";
                std::cout << "    str " << reg << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                regAlloc.free(reg);
                break;
            }

            case IRInstr::ldconstneg:
            {
                std::string reg = regAlloc.allocate();
                int val = (params[1][0] == '\'') ? (int)params[1][1] : stol(params[1]);
                std::cout << "    mov " << reg << ", #-" << val << "\n";
                std::cout << "    str " << reg << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                regAlloc.free(reg);
                break;
            }

            case IRInstr::copy:
            {
                std::string srcReg = regAlloc.allocate();
                std::cout << "    ldr " << srcReg << ", [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    str " << srcReg << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                regAlloc.free(srcReg);
                break;
            }

            case IRInstr::negexpr:
            {
                std::string reg = regAlloc.allocate();
                std::cout << "    ldr " << reg << ", [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    neg " << reg << ", " << reg << "\n";
                std::cout << "    str " << reg << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";
                regAlloc.free(reg);
                break;
            }

            case IRInstr::add:
            case IRInstr::sub:
            case IRInstr::mul:
            case IRInstr::div:
            case IRInstr::and_bit:
            case IRInstr::or_bit:
            case IRInstr::xor_bit:
            {
                std::string destReg = regAlloc.allocate();
                std::string srcReg1 = regAlloc.allocate();
                std::string srcReg2 = regAlloc.allocate();

                std::cout << "    ldr " << srcReg1 << ", [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr " << srcReg2 << ", [sp, #" << symbolTable->getOffset(params[2]) << "]\n";

                switch (op)
                {
                case IRInstr::add:
                    std::cout << "    add ";
                    break;
                case IRInstr::sub:
                    std::cout << "    sub ";
                    break;
                case IRInstr::mul:
                    std::cout << "    mul ";
                    break;
                case IRInstr::div:
                    std::cout << "    sdiv ";
                    break;
                case IRInstr::and_bit:
                    std::cout << "    and ";
                    break;
                case IRInstr::or_bit:
                    std::cout << "    orr ";
                    break;
                case IRInstr::xor_bit:
                    std::cout << "    eor ";
                    break;
                default:
                    break;
                }
                std::cout << destReg << ", " << srcReg1 << ", " << srcReg2 << "\n";

                std::cout << "    str " << destReg << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";

                regAlloc.free(destReg);
                regAlloc.free(srcReg1);
                regAlloc.free(srcReg2);
                break;
            }

            case IRInstr::mod:
            {
                std::string dividend = regAlloc.allocate();
                std::string divisor = regAlloc.allocate();
                std::string quotient = regAlloc.allocate();
                std::string temp = regAlloc.allocate();

                std::cout << "    ldr " << dividend << ", [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr " << divisor << ", [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    sdiv " << quotient << ", " << dividend << ", " << divisor << "\n";
                std::cout << "    mul " << temp << ", " << quotient << ", " << divisor << "\n";
                std::cout << "    sub " << dividend << ", " << dividend << ", " << temp << "\n";
                std::cout << "    str " << dividend << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";

                regAlloc.free(dividend);
                regAlloc.free(divisor);
                regAlloc.free(quotient);
                regAlloc.free(temp);
                break;
            }

            case IRInstr::inf:
            case IRInstr::sup:
            case IRInstr::eq:
            case IRInstr::diff:
            {
                std::string reg1 = regAlloc.allocate();
                std::string reg2 = regAlloc.allocate();
                std::string result = regAlloc.allocate();

                std::cout << "    ldr " << reg1 << ", [sp, #" << symbolTable->getOffset(params[1]) << "]\n";
                std::cout << "    ldr " << reg2 << ", [sp, #" << symbolTable->getOffset(params[2]) << "]\n";
                std::cout << "    cmp " << reg1 << ", " << reg2 << "\n";

                switch (op)
                {
                case IRInstr::inf:
                    std::cout << "    cset " << result << ", lt\n";
                    break;
                case IRInstr::sup:
                    std::cout << "    cset " << result << ", gt\n";
                    break;
                case IRInstr::eq:
                    std::cout << "    cset " << result << ", eq\n";
                    break;
                case IRInstr::diff:
                    std::cout << "    cset " << result << ", ne\n";
                    break;
                default:
                    break;
                }

                std::cout << "    str " << result << ", [sp, #" << symbolTable->getOffset(params[0]) << "]\n";

                regAlloc.free(reg1);
                regAlloc.free(reg2);
                regAlloc.free(result);
                break;
            }

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
                        int val = (params[0][0] == '\'') ? (int)params[0][1] : stol(params[0]);
                        std::cout << "    mov w0, #" << val << "\n";
                    }
                }

                int nbVariablesInScope = symbolTable->getNbVariablesInScope();
                int stackSize = (nbVariablesInScope + 2) * 4;
                stackSize = (stackSize + 15) & ~15;
                std::cout << "    add sp, sp, #" << stackSize << "\n";
                std::cout << "    ret\n";
                symbolTable->leaveScope();
                break;
            }

            case IRInstr::functionCall:
            {
                std::cout << "    mov x27, sp\n";
                // Engistrer les registres
                std::cout << "    stp x8, x9, [sp, #-16]!\n";
                std::cout << "    stp x10, x11, [sp, #-16]!\n";
                std::cout << "    stp x12, x13, [sp, #-16]!\n";

                //  Passer les parametres à w0 - w7
                // params[0]: type de reour; params[1]: valeur de retour; params[2]: lable de fonction
                vector<string> paramRegs = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7"};
                for (int i = 3; i < params.size(); i++)
                {
                    if (symbolTable->getOffset(params[i]) == -1)
                    {
                        std::cout << "    mov " << paramRegs[i - 3] << ", #" << params[i] << "\n";
                    }
                    else
                    {
                        std::cout << "    ldr " << paramRegs[i - 3] << ", [x27, #"
                                  << symbolTable->getOffset(params[i]) << "]\n";
                    }
                }

                std::cout << "    bl _" << params[2] << "\n";

                std::cout << "    ldp x12, x13, [sp], #16\n";
                std::cout << "    ldp x10, x11, [sp], #16\n";
                std::cout << "    ldp x8, x9, [sp], #16\n";

                if (params[0] != "void")
                {
                    std::cout << "    str w0, [x27, #" << symbolTable->getOffset(params[1]) << "]\n";
                }
                break;
            }

            case IRInstr::functionDef:
            {
                symbolTable->enterScope(params[0]);
                int nbVariablesInScope = symbolTable->getNbVariablesInScope();
                int stackSize = (nbVariablesInScope + 2) * 4;
                stackSize = (stackSize + 15) & ~15;

                std::cout << "_" << params[0] << ":\n";
                std::cout << "    sub sp, sp, #" << stackSize << "\n";

                std::cout << "    stp x29, x30, [sp, #" << stackSize - 16 << "]\n";
                std::cout << "    add x29, sp, #" << stackSize - 16 << "\n";

                vector<string> paramRegs = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7"};
                for (int i = 1; i < params.size(); i++)
                {
                    std::cout << "    str " << paramRegs[i - 1] << ", [sp, #"
                              << symbolTable->getOffset(params[i]) << "]\n";
                }
                break;
            }

            default:
                throw std::runtime_error("Unsupported operation in generateAssemblyArm64");
            }
        }
    }
}