#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <string>
#include <vector>

struct Variable {
    std::string name;
    int index;

    Variable(std::string name, int index) : name(name), index(index) {}
};

class  CodeGenSymboleTable : public ifccBaseVisitor {
	public:
        CodeGenSymboleTable(std::vector<Variable> & s):symboleTable(s){};

        virtual antlrcpp::Any visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) override;
        virtual antlrcpp::Any visitDeclarationVaV(ifccParser::DeclarationVaVContext * ctx) override;
        virtual antlrcpp::Any visitDeclarationV(ifccParser::DeclarationVContext *ctx) override;
        virtual antlrcpp::Any visitAffectationVaC(ifccParser::AffectationVaCContext *ctx) override;
        bool isInTable(std::string varName);

    private :
        std::vector<Variable> symboleTable;
};