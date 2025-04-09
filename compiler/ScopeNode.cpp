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
    // renvoie un pointeur vers la variable nommée "name" dans le scope, nullptr si elle n'existe pas
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
    // renvoie un pointeur vers le FunctionScopeNode appartenant aux ancêtres du scope
    ScopeNode* current = this;
    while (current) {
        FunctionScopeNode* functionScope = dynamic_cast<FunctionScopeNode*>(current);
        if (functionScope) {
            return functionScope;
        }
        current = current->getParent();
    }
    string erreur = "error: no function found\n";
    throw runtime_error(erreur);
    return nullptr;
}

variable* ScopeNode::addVariable(string name, int offset, bool use, bool init, bool isTemp) 
{
    variable var;
    var.name = name;
    var.offset = offset;
    var.use = use;
    var.init = init;
    var.isTemp = isTemp;
    variable_vect.push_back(var);
    return &(variable_vect.back());
}

void ScopeNode::checkIfEachIdUsed() {
    // vérifie que toutes les variables du scope sont utilisées dans le programme, i.e. font l'office de rvalue, sinon quoi un warning est renvoyé à l'utilisateur
    for (const auto& var : variable_vect) {
        if (var.isTemp == false && var.use == false) {
            cerr << "WARNING : variable " << var.name << " not used" << endl;
        }
    }
}

void ScopeNode::checkIfEachIdInit() {
    // vérifie que toutes les variables du scope utilisées dans le programme ont bien été initialisées, sinon quoi un warning est renvoyé à l'utilisateur
    for (const auto& var : variable_vect) {
        if (var.isTemp == false && var.init == false) 
        {
            cerr << "WARNING : variable " << var.name << " declared but not initialized" << endl;
        }
    }
}

variable* ScopeNode::getNextNotUsedTempVar() {
    // renvoie le nom de la prochaine variable temporaire appartenant au scope n'ayant pas encore été utilisée
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
