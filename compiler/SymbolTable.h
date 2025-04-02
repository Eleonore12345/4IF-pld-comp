#pragma once

#include "ScopeNode.h"

#include <vector>
#include <string>

using namespace std;

class SymbolTable {
    public:
        SymbolTable();
        virtual ~SymbolTable();
        void print();
        ScopeNode* getCurrentScope();
        variable* getVariable(string name);
        void checkIfEachIdUsed();
        void checkIfEachIdInit();
        void createAndEnterFunctionScope(string name);
        void createAndEnterScope();
        void enterNextScope();
        void leaveScope();
        void resetAndRootToCurrent();

    private:
        void printScope(ScopeNode* scope, int level);
        void checkIfEachIdUsedInScope(ScopeNode* scope);
        void checkIfEachIdInitInScope(ScopeNode* scope);
        void freeScopes(ScopeNode* scope);
        void reset(ScopeNode* scope);

        ScopeNode* rootScope;
        ScopeNode* currentScope;
};
