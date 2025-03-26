#include "FunctionTable.h"

FunctionTable::FunctionTable() {
    function_identifier f1;
    f1.functionName = "putchar";
    f1.nbParams = 1;
    f1.def = true;

    function_identifier f2;
    f2.functionName = "getchar";
    f2.nbParams = 0;
    f2.def = true;

    addFunction(f1);
    addFunction(f2);
}

void FunctionTable::addFunction(function_identifier f) 
{
    vect.push_back(f);
}

int FunctionTable::getIndex(string name) 
{
    for (int i = 0 ; i < vect.size() ; i++)
    {
        if (vect[i].functionName == name) 
        {
            return i;
        }
    }
    return -1;
}

void FunctionTable::setDef(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].def = true;
    }
}

int FunctionTable::getNbParams(string name) {
    int index = getIndex(name);
    if (index != -1) {
        return vect[index].nbParams;
    }
    return -1;
}

bool FunctionTable::isDefined(string name) {
    int index = getIndex(name);
    if (index != -1) {
        return vect[index].def;
    }
    return false;
}