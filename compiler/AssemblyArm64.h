#pragma once

#include <vector>
#include <string>

#include "SymbolTable.h"
#include "IR.h"

using namespace std;

// classe qui va générer le code assembleur pour x86
class AssemblyArm64
{
public:
    AssemblyArm64(CFG *c, SymbolTable *s) : cfgArm64(c), symbolTable(s) {};
    virtual ~AssemblyArm64() {};

    void generateAssemblyArm64();

private:
    CFG *cfgArm64;
    SymbolTable *symbolTable;
};
