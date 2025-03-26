#pragma once

#include <vector>
#include <string>

using namespace std;

typedef struct {
    string functionName;
    int nbParams;
    bool def = false;
} function_identifier;

class FunctionTable {
    public:
        FunctionTable();
        virtual ~FunctionTable(){};
        void addFunction(function_identifier f);
        void setDef(string name);
        int getIndex(string name); 
        int getNbParams(string name);
        bool isDefined(string name);

    private:
        vector<function_identifier> vect;
};
