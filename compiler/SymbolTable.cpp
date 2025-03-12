#include "SymbolTable.h"
#include <iostream>

using namespace std;

void SymbolTable::print()
{
    for (auto a : vect){
        cout << "id : " << a.identifier << " index : " << a.index << endl;
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

void SymbolTable::setUse(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].use = true;
    }
}

bool SymbolTable::isEachIdUsed() {
    bool res = true;
    for (auto a : vect)
    {
        if (a.use == false) 
        {
            //cout << "Variable " << a.identifier << " not used" << endl;
            res = false;
        }
    }
    return res;
}