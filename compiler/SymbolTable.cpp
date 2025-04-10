#include "SymbolTable.h"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable() {
    // Création du scope racine : celui du fichier
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
    // crée un nouveau FunctionScopeNode comme enfant du scope courant
    ScopeNode* nextScope = new FunctionScopeNode(currentScope, name);
    currentScope->addChildren(nextScope);
    // le scope créé devient le scope courant
    currentScope = nextScope;
}

void SymbolTable::createAndEnterScope() {
    // crée un nouveau ScopeNode comme enfant du scope courant
    ScopeNode * nextScope = new ScopeNode(currentScope);
    currentScope->addChildren(nextScope);
    // le scope créé devient le scope courant
    currentScope = nextScope;
}

void SymbolTable::enterNextScope() {
    // le prochain scope qui doit être visité devient le scope courant
    bool found = false;
    for (ScopeNode* scope : currentScope->getChildren()) {
        // le prochain scope qui doit être visité et le premier enfant non visité du scope
        if (scope->isVisited() == false) 
        {
            currentScope = scope;
            found = true;
            break;
        }
    }
    if(!found) {
        string erreur = "error : no unvisited scope left\n";
        throw runtime_error(erreur);
    }
}

void SymbolTable::leaveScope() {
    // quitter un scope revient à revenir dans son scope parent
    currentScope = currentScope->getParent();
}

void SymbolTable::resetAndRootToCurrent() {
    // le scope courant devient le scope racine, et tous les scopes de la table sont marqués comme non-visités
    currentScope = rootScope;
    reset(currentScope);
}

void SymbolTable::reset(ScopeNode* scope) {
    // à partir du scope passé en paramètre, tous les scopes de sa hiérarchie descendante sont marqués comme non-visités
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
    // renvoie un pointeur vers la variable nommée "name" dans le scope courant et sa hiérarchie ascendante, nullptr si elle n'existe pas
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
    // vérifie que toutes les variables du programme sont utilisées dans le programme, i.e. font l'office de rvalue
    checkIfEachIdUsedInScope(rootScope);
}

void SymbolTable::checkIfEachIdInit() {
    // vérifie que toutes les variables du programme ont bien été initialisées
    checkIfEachIdInitInScope(rootScope);
}

void SymbolTable::checkIfEachIdUsedInScope(ScopeNode* scope) {
    // vérifie que toutes les variables du scope passé en paramètre et des scopes de sa hiérarchie descendante sont utilisées dans le programme, i.e. font l'office de rvalue
    scope->checkIfEachIdUsed();
    for (ScopeNode* child : scope->getChildren()) {
        checkIfEachIdUsedInScope(child);
    }
}

void SymbolTable::checkIfEachIdInitInScope(ScopeNode* scope) {
    // vérifie que toutes les variables du scope passé en paramètre et des scopes de sa hiérarchie descendante utilisées dans le programme ont bien été initialisées
    scope->checkIfEachIdInit();
    for (ScopeNode* child : scope->getChildren()) {
        checkIfEachIdInitInScope(child);
    }
}