#pragma once

#include <vector>
#include <string>

#include "SymbolTable.h"
#include "IR.h"

using namespace std;

class AssemblyArm64 {
    public:
        AssemblyArm64(vector<CFG*> c, SymbolTable* s);
        virtual ~AssemblyArm64(){};

        void generateAssemblyArm64();

    private:
        vector<CFG*> cfgsArm64;
        SymbolTable * symbolTable;
};