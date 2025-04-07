#include "SymbolTable.h"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable() {
    string fichier = "scope_fichier";
    currentScope = new FunctionScopeNode(nullptr, fichier);
    rootScope = currentScope;
}

SymbolTable::~SymbolTable() {
    freeScopes(rootScope);
}

void SymbolTable::freeScopes(ScopeNode * scope) {
    if (scope == nullptr) {
        return;
    }
    for (ScopeNode* child : scope->getChildren()) {
        freeScopes(child);
    }
    delete scope;
}

void SymbolTable::createAndEnterFunctionScope(string name) {
    ScopeNode* nextScope = new FunctionScopeNode(currentScope, name);
    currentScope->addChildren(nextScope);
    currentScope = nextScope;
}

void SymbolTable::createAndEnterScope() {
    ScopeNode * nextScope = new ScopeNode(currentScope);
    currentScope->addChildren(nextScope);
    currentScope = nextScope;
}

void SymbolTable::enterNextScope() {
    bool found = false;
    for (ScopeNode* a : currentScope->getChildren()) {
        if (a->isVisited() == false) 
        {
            currentScope = a;
            found = true;
            break;
        }
    }
    if(!found) {
        string erreur = "Error : no unvisited scope left\n";
        //print();
        throw runtime_error(erreur);
    }
}

void SymbolTable::leaveScope() {
    currentScope = currentScope->getParent();
}

void SymbolTable::resetAndRootToCurrent() {
    currentScope = rootScope;
    reset(currentScope);
}

void SymbolTable::reset(ScopeNode* scope) {
    scope->reset();
    for (ScopeNode* child : scope->getChildren()) {
        reset(child);
    }
}

void SymbolTable::print() {
    if (rootScope) {
        printScope(rootScope,0);
    }
}

ScopeNode* SymbolTable::getCurrentScope() {
    return currentScope;
}

void SymbolTable::printScope(ScopeNode* scope, int level) {
    cout << "Level : " << level << endl;
    scope->print();
    for (ScopeNode* child : scope->getChildren()) {
        printScope(child, level + 1);
    }
}

variable* SymbolTable::getVariable(string name) 
{
    ScopeNode* scope = currentScope;
    while (scope != nullptr){
        variable* variable = scope->getVariable(name);
        if (variable) {
            return variable;
        }
        scope = scope->getParent();
    }
    return nullptr;
}

void SymbolTable::checkIfEachIdUsed() {
    checkIfEachIdUsedInScope(rootScope);
}

void SymbolTable::checkIfEachIdInit() {
    checkIfEachIdInitInScope(rootScope);
}

void SymbolTable::checkIfEachIdUsedInScope(ScopeNode* scope) {
    scope->checkIfEachIdUsed();
    for (ScopeNode* child : scope->getChildren()) {
        checkIfEachIdUsedInScope(child);
    }
}

void SymbolTable::checkIfEachIdInitInScope(ScopeNode* scope) {
    scope->checkIfEachIdInit();
    for (ScopeNode* child : scope->getChildren()) {
        checkIfEachIdInitInScope(child);
    }
}