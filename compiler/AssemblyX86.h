#pragma once

#include <vector>
#include <string>

#include "SymbolTable.h"
#include "IR.h"

using namespace std;


// classe qui va générer le code assembleur pour x86
class AssemblyX86 {
    public:
        AssemblyX86(vector<CFG*> c, SymbolTable* s);
        virtual ~AssemblyX86(){};

        void generateAssemblyX86();

    private:
        vector<CFG*> cfgsX86;
        SymbolTable * symbolTable;
};
