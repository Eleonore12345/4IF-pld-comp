#include "SymbolTable.h"
#include <iostream>

using namespace std;

SymbolTable::SymbolTable() {
    currentScope = new scopeNode();
    currentScope->nameScope = "scope_fichier";
    currentScope->parent = nullptr;
    rootScope = currentScope;
}

SymbolTable::~SymbolTable() {
    freeScopes(rootScope);
}

void SymbolTable::freeScopes(scopeNode * scope) {
    if (scope == nullptr) {
        return;
    }
    for (auto* child : scope->children) {
        freeScopes(child);
    }
    delete scope;
}

void SymbolTable::createAndEnterScope(string name) {
    scopeNode * nextScope = new scopeNode();
    nextScope->nameScope = name;
    nextScope->parent = currentScope;
    currentScope->children.push_back(nextScope);
    currentScope = nextScope;
}

void SymbolTable::enterScope(string name) {
    bool found = false;
    for(const auto& a : currentScope->children) {
        if(a->nameScope == name) {
            currentScope = a;
            found = true;
            break;
        }
    }
    if(!found) {
        string erreur = "Error : Scope " + name + " not found from Scope " + currentScope->nameScope + " !";
        throw runtime_error(erreur);
    }
}

void SymbolTable::leaveScope() {
    currentScope = currentScope->parent;
}

void SymbolTable::rootToCurrent() {
    currentScope = rootScope;
}

void SymbolTable::print() {
    if(rootScope) {
        printScope(rootScope,0);
    }
}

void SymbolTable::printCurrentScope() {
    cout << currentScope->nameScope << endl;
    for(const auto & a : currentScope->vect) {
        cout << "   id : " << a.identifier << " offset : " << a.offset << endl;
    }
}

void SymbolTable::printScope(scopeNode* scope, int level) {
    cout << "Level : " << level << ", Scope: " << scope->nameScope << endl;
    for(const auto & a : scope->vect) {
        cout << "   id : " << a.identifier << " offset : " << a.offset << " used : " << a.use << endl;
    }
    for (auto* child : scope->children) {
        printScope(child, level + 1);
    }
}

void SymbolTable::addIdentifier(desc_identifier id) 
{
    currentScope->vect.push_back(id);
}

int SymbolTable::size() 
{
    return currentScope->vect.size();
}

int SymbolTable::getIndex(string name) 
{
    scopeNode* scope = currentScope;
    while (scope != nullptr){
        for (int i = 0 ; i < scope->vect.size() ; i++)
        {
            if ((scope->vect)[i].identifier == name) 
            {
                return i;
            }
        }
        scope = scope->parent;
    }
    return -1;
}

int SymbolTable::getOffset(string name)
{
    scopeNode* scope = currentScope;
    while (scope != nullptr){
        for(int i = 0 ; i < scope->vect.size() ; i++)
        {
            if ((scope->vect)[i].identifier == name) 
            {
                return (scope->vect)[i].offset;
            }
        }
        scope = scope->parent;
    }
    return -1;
}

int SymbolTable::getInitStatus(string name) {
    scopeNode* scope = currentScope;
    while (scope != nullptr){
        for(int i = 0 ; i < scope->vect.size() ; i++)
        {
            if ((scope->vect)[i].identifier == name) 
            {
                return (scope->vect)[i].init;
            }
        }
        scope = scope->parent;
    }
    return -1;
}

void SymbolTable::setUse(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        (currentScope->vect)[index].use = true;
    }
}

void SymbolTable::setInit(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        (currentScope->vect)[index].init = true;
    }
}

void SymbolTable::checkIfEachIdUsed() {
    checkIfEachIdUsedInScope(rootScope);
}

void SymbolTable::checkIfEachIdInit() {
    checkIfEachIdInitInScope(rootScope);
}

void SymbolTable::checkIfEachIdUsedInScope(scopeNode* scope) {
    for (const auto& a : scope->vect) {
        if (a.use == false && a.isTemp == false) {
            cerr << "WARNING : variable " << a.identifier << " not used" << endl;
        }
    }
    for (auto* child : scope->children) {
        checkIfEachIdUsedInScope(child);
    }
}

void SymbolTable::checkIfEachIdInitInScope(scopeNode* scope) {
    for (const auto& a : scope->vect) {
        if (a.init == false) 
        {
            cerr << "WARNING : variable " << a.identifier << " declared but not initialized" << endl;
        }
    }
    for (auto* child : scope->children) {
        checkIfEachIdInitInScope(child);
    }
}

string SymbolTable::getNextNotUsedTempVar() {
    for (const auto& a : currentScope->vect) {
        if (a.isTemp == true && a.use == false) 
        {
            return a.identifier;
        }
    }
    string erreur = "No temp variable available\n";
    throw runtime_error(erreur);
    return "";
}

int SymbolTable::getNbVariablesInScope() {
    return currentScope->vect.size();
}