#include "FunctionTable.h"
#include <stdexcept>
#include <iostream>

FunctionTable::FunctionTable() {
    function_identifier f1;
    f1.functionName = "putchar";
    f1.retourType = "int";
    f1.nbParams = 1;
    f1.def = true;

    function_identifier f2;
    f2.functionName = "getchar";
    f2.retourType = "int";
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

void FunctionTable::setReturnType(string name, string type) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].retourType = type;
    }
}

void FunctionTable::setCurrentFunction(string name) {
    currentFunction = name;
}

void FunctionTable::setAsRval(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].rval = true;
    }
}

string FunctionTable::getCurrentFunction() {
    return currentFunction;
}

int FunctionTable::getNbParams(string name) {
    int index = getIndex(name);
    if (index != -1) {
        return vect[index].nbParams;
    }
    return -1;
}

string FunctionTable::getReturnType(string name) {
    int index = getIndex(name);
    if (index != -1) {
        return vect[index].retourType;
    }
    return "";
}

bool FunctionTable::isDefined(string name) {
    int index = getIndex(name);
    if (index != -1) {
        return vect[index].def;
    }
    return false;
}

bool FunctionTable::isPresent(string name) {
    int index = getIndex(name);
    if (index != -1) {
        return true;
    }
    return false;
}

void FunctionTable::checkIfEachFuncDefined() {
    for (auto a : vect)
    {
        if (a.def == false) 
        {
            std::string erreur = "undefined reference to " + a.functionName + "\n";
            throw std::runtime_error(erreur);
        }
    }
}

void FunctionTable::checkRvalFuncReturnType() {
    for (auto a : vect)
    {
        if (a.rval == true && a.retourType == "void") 
        {
        std::cerr << "WARNING : conflicting types for '" << a.functionName << "'; have ‘void()’" << std::endl;
        }
    }
}

void FunctionTable::setHasReturnTrue(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        vect[index].hasReturn = true;
    }
}

bool FunctionTable::hasReturn(string name) {
    int index = getIndex(name);
    if (index != -1) 
    {
        return vect[index].hasReturn;
    } 
    return false;
}