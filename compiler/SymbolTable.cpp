#include "SymbolTable.h"
#include <iostream>

using namespace std;

void SymbolTable::print()
{
    for (auto a : vect){
        cout << "id : " << a.identifier << " index : " << a.offset << endl;
    }
}

void SymbolTable::addIdentifier(desc_identifier id) 
{
    vect.push_back(id);
}

int SymbolTable::size() 
{
    return vect.size();
}

int SymbolTable::getIndex(string name) 
{
    for (int i = 0 ; i < vect.size() ; i++)
    {
        if (vect[i].identifier == name) 
        {
            return i;
        }
    }
    return -1;
}

int SymbolTable::getOffset(string name)
{
    for(int i = 0 ; i < vect.size() ; i++)
    {
        if (vect[i].identifier == name) 
        {
            return vect[i].offset;
        }
    }
    return -1;
}

int SymbolTable::getInitStatus(string name) {
    for(int i = 0 ; i < vect.size() ; i++)
    {
        if (vect[i].identifier == name) 
        {
            return vect[i].init;
        }
    }
    return -1;
}

void SymbolTable::setUse(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].use = true;
    }
}

void SymbolTable::setInit(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].init = true;
    }
}

void SymbolTable::checkIfEachIdUsed() {
    for (auto a : vect)
    {
        if (a.use == false) 
        {
            cerr << "WARNING : variable " << a.identifier << " not used" << endl;
        }
    }
}

void SymbolTable::checkIfEachIdInit() {
    for (auto a : vect)
    {
        if (a.init == false) 
        {
            cerr << "WARNING : variable " << a.identifier << " declared but not initialized" << endl;
        }
    }
}