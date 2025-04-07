#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct {
    string functionName;
    string retourType;
    int nbParams;
    bool def = false;
    bool rval = false;
    bool hasReturn = false;
} function_identifier;

class FunctionTable {
    public:
        FunctionTable();
        virtual ~FunctionTable(){};
        void addFunction(function_identifier f);
        void setDef(string name);
        void setReturnType(string name, string type);
        void setHasReturnTrue(string name);
        void setCurrentFunction(string name);
        void setAsRval(string name);
        string getCurrentFunction();
        int getIndex(string name); 
        int getNbParams(string name);
        string getReturnType(string name);
        bool isDefined(string name);
        bool isPresent(string name);
        bool hasReturn(string name);
        void checkIfEachFuncDefined();
        void checkRvalFuncReturnType();

    private:
        vector<function_identifier> vect;
        string currentFunction;
};
