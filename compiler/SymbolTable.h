#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct {
    string identifier;
    int offset;
    bool use = false;
    bool init = false;
    bool isTemp = false;
} desc_identifier;

typedef struct scopeNode {
    string nameScope;
    vector<desc_identifier> vect;
    scopeNode* parent;
    vector<scopeNode*> children;
    bool visited = false;
} scopeNode;

class SymbolTable {
    public:
        SymbolTable();
        virtual ~SymbolTable();
        void print();
        string getCurrentScope();
        int size(string functionName);
        void addIdentifier(desc_identifier id);
        int getIndex(string name);
        int getIndexInScope(string name);
        int getOffset(string name);
        int getInitStatus(string name);
        void setUse(string name);
        void setInit(string name);
        void setCurrentScopeVisited();
        void checkIfEachIdUsed();
        void checkIfEachIdInit();
        void createAndEnterScope(string name);
        void enterNextScope();
        void leaveScope();
        void resetAndRootToCurrent();
        string getNextNotUsedTempVar();
        int getNbVariablesInScope();
        scopeNode* getScope(string name);

    private:
        void printScope(scopeNode* scope, int level);
        void checkIfEachIdUsedInScope(scopeNode* scope);
        void checkIfEachIdInitInScope(scopeNode* scope);
        void freeScopes(scopeNode* scope);
        void reset(scopeNode* scope);
        void addScopeSize(scopeNode* rootScope, string functionName, int* size);

        scopeNode * rootScope;
        scopeNode * currentScope;
};
