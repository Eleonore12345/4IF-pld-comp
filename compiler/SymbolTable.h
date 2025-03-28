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
} scopeNode;

class SymbolTable {
    public:
        SymbolTable();
        virtual ~SymbolTable();
        void print();
        void printCurrentScope();
        int size();
        void addIdentifier(desc_identifier id);
        int getIndex(string name);
        int getOffset(string name);
        int getInitStatus(string name);
        void setUse(string name);
        void setInit(string name);
        void checkIfEachIdUsed();
        void checkIfEachIdInit();
        void createAndEnterScope(string name);
        void enterScope(string name);
        void leaveScope();
        void rootToCurrent();
        string getNextNotUsedTempVar();
        int getNbVariablesInScope();

    private:
        void printScope(scopeNode* scope, int level);
        void checkIfEachIdUsedInScope(scopeNode* scope);
        void checkIfEachIdInitInScope(scopeNode* scope);
        void freeScopes(scopeNode* scope);

        scopeNode * rootScope;
        scopeNode * currentScope;
};
