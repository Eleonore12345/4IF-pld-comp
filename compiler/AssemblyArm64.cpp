#include "AssemblyArm64.h"
#include <vector>
#include <string>
#include <iostream>
#include <stack>

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
            return "spill";
        std::string reg = freeRegisters.top();
        freeRegisters.pop();
        return reg;
    }

    void free(const std::string &reg)
    {
        if (reg != "spill")
            freeRegisters.push(reg);
    }
};

AssemblyArm64::AssemblyArm64(std::vector<CFG *> c, SymbolTable *s)
{
    this->cfgsArm64 = c;
    this->symbolTable = s;
    s->resetAndRootToCurrent();
}

void AssemblyArm64::generateAssemblyArm64()
{
    std::cout << ".globl main\n";
    RegisterAllocator regAlloc;

    for (CFG *c : cfgsArm64)
    {
        vector<BasicBlock *> bbs = c->get_bbs();
        for (BasicBlock *bb : c->get_bbs())
        {
            for (IRInstr *instr : bb->instrs)
            {
                IRInstr::Operation op = instr->getOperation();
                vector<string> params = instr->getParams();
                auto getOffset = [&](const std::string &name)
                {
                    return -symbolTable->getVariable(name)->offset;
                };

                auto loadReg = [&](const std::string &var)
                {
                    std::string reg = regAlloc.allocate();
                    std::cout << "    ldr " << reg << ", [sp, #" << getOffset(var) << "]\n";
                    return reg;
                };

                switch (op)
                {
                case IRInstr::ldconst:
                case IRInstr::ldconstneg:
                {
                    std::string reg = regAlloc.allocate();
                    int val = (params[1][0] == '\'') ? (int)params[1][1] : std::stol(params[1]);
                    std::cout << "    mov " << reg << ", #" << ((op == IRInstr::ldconstneg) ? -val : val) << "\n";
                    std::cout << "    str " << reg << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(reg);
                    break;
                }

                case IRInstr::copy:
                {
                    std::string srcReg = loadReg(params[1]);
                    std::cout << "    str " << srcReg << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(srcReg);
                    break;
                }
                case IRInstr::notconst:
                {
                    std::string reg = regAlloc.allocate();
                    int val = (params[1][0] == '\'') ? (int)params[1][1] : std::stol(params[1]);
                    std::cout << "    mov " << reg << ", #" << val << "\n";
                    std::cout << "    cmp " << reg << ", #0\n";
                    std::cout << "    cset " << reg << ", eq\n";
                    std::cout << "    str " << reg << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(reg);
                    break;
                }
                case IRInstr::notexpr:
                {
                    std::string r = loadReg(params[1]);
                    std::cout << "    cmp " << r << ", #0\n";
                    std::cout << "    cset " << r << ", eq\n";
                    std::cout << "    str " << r << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(r);
                    break;
                }
                case IRInstr::negexpr:
                {
                    std::string reg = loadReg(params[0]);
                    std::cout << "    neg " << reg << ", " << reg << "\n";
                    std::cout << "    str " << reg << ", [sp, #" << getOffset(params[0]) << "]\n";
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
                    std::string dst = regAlloc.allocate();
                    std::string r1 = loadReg(params[1]);
                    std::string r2 = loadReg(params[2]);
                    const char *opStr = (op == IRInstr::add) ? "add" : (op == IRInstr::sub)   ? "sub"
                                                                   : (op == IRInstr::mul)     ? "mul"
                                                                   : (op == IRInstr::div)     ? "sdiv"
                                                                   : (op == IRInstr::and_bit) ? "and"
                                                                   : (op == IRInstr::or_bit)  ? "orr"
                                                                                              : "eor";
                    std::cout << "    " << opStr << " " << dst << ", " << r1 << ", " << r2 << "\n";
                    std::cout << "    str " << dst << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(dst);
                    regAlloc.free(r1);
                    regAlloc.free(r2);
                    break;
                }

                case IRInstr::mod:
                {
                    std::string dividend = loadReg(params[1]);
                    std::string divisor = loadReg(params[2]);
                    std::string q = regAlloc.allocate(), tmp = regAlloc.allocate();
                    std::cout << "    sdiv " << q << ", " << dividend << ", " << divisor << "\n";
                    std::cout << "    mul " << tmp << ", " << q << ", " << divisor << "\n";
                    std::cout << "    sub " << dividend << ", " << dividend << ", " << tmp << "\n";
                    std::cout << "    str " << dividend << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(dividend);
                    regAlloc.free(divisor);
                    regAlloc.free(q);
                    regAlloc.free(tmp);
                    break;
                }

                case IRInstr::inf:
                case IRInstr::sup:
                case IRInstr::eq:
                case IRInstr::diff:
                {
                    std::string r1 = loadReg(params[1]);
                    std::string r2 = loadReg(params[2]);
                    std::string result = regAlloc.allocate();
                    std::cout << "    cmp " << r1 << ", " << r2 << "\n";
                    const char *cond = (op == IRInstr::inf) ? "lt" : (op == IRInstr::sup) ? "gt"
                                                                 : (op == IRInstr::eq)    ? "eq"
                                                                                          : "ne";
                    std::cout << "    cset " << result << ", " << cond << "\n";
                    std::cout << "    str " << result << ", [sp, #" << getOffset(params[0]) << "]\n";
                    regAlloc.free(r1);
                    regAlloc.free(r2);
                    regAlloc.free(result);
                    break;
                }

                case IRInstr::retour:
                {
                    if (!params.empty())
                    {
                        auto *var = symbolTable->getVariable(params[0]);
                        if (var)
                            std::cout << "    ldr w0, [sp, #" << -var->offset << "]\n";
                        else
                            std::cout << "    mov w0, #" << params[0] << "\n";
                    }
                    auto *func = (FunctionScopeNode *)symbolTable->getCurrentScope();
                    int sizeSub = func->getSize() + 4;
                    while (sizeSub % 16 != 0)
                        sizeSub += 4;
                    // std::cout << "    ldp x29, x30, [sp, #" << sizeSub - 16 << "]\n";
                    std::cout << "    add sp, sp, #" << sizeSub << "\n    ret\n";
                    break;
                }

                case IRInstr::functionCall:
                {
                    std::cout << "    mov x27, sp\n";
                    std::vector<std::string> args = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7"};
                    for (int i = 3; i < params.size(); i++)
                    {
                        auto *var = symbolTable->getVariable(params[i]);
                        if (var)
                            std::cout << "    ldr " << args[i - 3] << ", [x27, #" << var->offset << "]\n";
                        else
                            std::cout << "    mov " << args[i - 3] << ", #" << params[i] << "\n";
                    }
                    std::cout << "    bl " << params[2] << "\n";
                    if (params[0] != "void") // return value is not void
                    {
                        auto *var = symbolTable->getVariable(params[1]);
                        if (var)
                            std::cout << "    str w0, [x27, #" << -var->offset << "]\n";
                    }

                    break;
                }

                case IRInstr::functionDef:
                {
                    auto *func = (FunctionScopeNode *)symbolTable->getCurrentScope();
                    int sizeSub = func->getSize() + 4;
                    while (sizeSub % 16 != 0)
                        sizeSub += 4;
                    std::cout << params[0] << ":\n    sub sp, sp, #" << sizeSub << "\n";
                    std::cout << "    stp x29, x30, [sp, #" << sizeSub - 16 << "]\n";
                    std::cout << "    add x29, sp, #" << sizeSub - 16 << "\n";
                    std::vector<std::string> args = {"w0", "w1", "w2", "w3", "w4", "w5", "w6", "w7"};
                    for (int i = 1; i < params.size(); i++)
                    {
                        auto *var = symbolTable->getVariable(params[i]);
                        if (var)
                            std::cout << "    str " << args[i - 1] << ", [sp, #" << -var->offset << "]\n";
                    }
                    break;
                }

                case IRInstr::enter_bloc:
                    // symbolTable->print();
                    symbolTable->enterNextScope();
                    break;

                case IRInstr::leave_bloc:
                    symbolTable->getCurrentScope()->setVisited();
                    symbolTable->leaveScope();
                    break;

                default:
                    throw std::runtime_error("Unsupported operation in generateAssemblyArm64");
                }
            }
            if (bb->exit_true)
            {
                if (bb->exit_false)
                {
                    std::string label_bb_true = bb->exit_true->label;
                    std::string label_bb_false = bb->exit_false->label;
                    std::cout << "    cmp w0, #0\n";
                    std::cout << "    bne " <<label_bb_true << "\n";
                    std::cout << "    b " << label_bb_false << "\n";
                }
                else
                {
                    std::string label_bb_endif = bb->exit_true->label;
                    std::cout << "    b " << label_bb_endif << "\n";
                }
            }
        }

        // on quitte le scope de la fonction
        symbolTable->getCurrentScope()->setVisited();
        symbolTable->leaveScope();
    }
}
