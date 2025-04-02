#include "ScopeNode.h"
#include <iostream>

using namespace std;

ScopeNode::ScopeNode(ScopeNode* parent) {
    visited = false;
    this->parent = parent;
    variable_vect = {};
    children = {};
}

void ScopeNode::reset() {
    visited = false;
}

void ScopeNode::print() {
    cout << "Scope: " <<  endl;
    cout << "   Visited:" << visited << endl;
    cout << "   Number of children:" << children.size() << endl;
    cout << "   Function parent:" << getFunctionParent()->getName() << endl;
    cout << "   Variable(s):" << endl;
    for(const auto & var : variable_vect) {
        cout << "       name : " << var.name << " offset : " << var.offset << " used : " << var.use << " init : " << var.init << endl;
    }
}

variable* ScopeNode::getVariable(string name) {
    for (int i = 0; i < variable_vect.size() ; i++) {
        if (variable_vect[i].name == name) {
            return &(variable_vect[i]);
        }
    }
    return nullptr;
}

ScopeNode* ScopeNode::getParent() {
    return parent;
}

vector<ScopeNode*> ScopeNode::getChildren() {
    return children;
}

FunctionScopeNode* ScopeNode::getFunctionParent() {
    ScopeNode* current = this;
    while (current) {
        FunctionScopeNode* functionScope = dynamic_cast<FunctionScopeNode*>(current);
        if (functionScope) {
            return functionScope;
        }
        current = current->getParent();
    }
    return nullptr; // Aucun FunctionScopeNode trouvé
}

void ScopeNode::addVariable(string name, bool use, bool init, bool isTemp) 
{
    FunctionScopeNode* functionParent = getFunctionParent();
    functionParent->incrementSize(4);
    variable var;
    var.name = name;
    var.offset = functionParent->getSize();
    var.use = use;
    var.init = init;
    var.isTemp = isTemp;
    variable_vect.push_back(var);
}

void ScopeNode::checkIfEachIdUsed() {
    for (const auto& var : variable_vect) {
        if (var.isTemp == false && var.use == false) {
            cerr << "WARNING : variable " << var.name << " not used" << endl;
        }
    }
}

void ScopeNode::checkIfEachIdInit() {
    for (const auto& var : variable_vect) {
        if (var.isTemp == false && var.init == false) 
        {
            cerr << "WARNING : variable " << var.name << " declared but not initialized" << endl;
        }
    }
}

variable* ScopeNode::getNextNotUsedTempVar() {
    for (auto& var : variable_vect) {
        if (var.isTemp == true && var.use == false) 
        {
            return &var;
        }
    }
    string erreur = "error: no temp variable available\n";
    throw runtime_error(erreur);
    return nullptr;
}

void ScopeNode::setVisited() {
    visited = true;
}

bool ScopeNode::isVisited() {
    return visited;
}

void ScopeNode::addChildren(ScopeNode* children) {
    if (children) {
        this->children.push_back(children);
    }
}

int ScopeNode::getNbTmpVariable() {
    int value = 0;
    for(const auto & var : variable_vect) {
      if (var.isTemp) {
            value++;
      }
    }
    return value;
}

void FunctionScopeNode::print(){
    cout << "Scope: " <<  endl;
    cout << "   Visited:" << visited << endl;
    cout << "   Number of children:" << children.size() << endl;
    cout << "   Function name:" << name << endl;
    cout << "   Size:" << size << endl;
    cout << "   Variable(s):" << endl;
    for(const auto & var : variable_vect) {
        cout << "       name : " << var.name << " offset : " << var.offset << " used : " << var.use << " init : " << var.init << endl;
    }
}

string FunctionScopeNode::getName() const {
    return name;
}

void FunctionScopeNode::incrementSize(int value) {
    size += value;
}

int FunctionScopeNode::getSize() {
    return size;
}
