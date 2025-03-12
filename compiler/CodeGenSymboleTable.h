#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <string>

struct Variable {
    std::string name;
    int index;
};

class  CodeGenSymboleTable : public ifccBaseVisitor {
	public:
        CodeGenSymboleTable(std::vector<Variable> * s):symboleTable(s){};

    private :
        std::vector<Variable> * symboleTable;
};