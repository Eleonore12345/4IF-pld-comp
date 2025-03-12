#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include <string>
#include <vector>

struct Variable {
    std::string name;
    int index;

    Variable(std::string name, int index): name(name), index(index) {}
};

class  CodeGenSymbolTable : public ifccBaseVisitor {
	public:
        CodeGenSymbolTable(std::vector<Variable> & s):symbolTable(s){};

        virtual antlrcpp::Any visitDeclarationV(ifccParser::DeclarationVContext *ctx) override;
        virtual antlrcpp::Any visitDeclarationVaV(ifccParser::DeclarationVaVContext *ctx) override;
        virtual antlrcpp::Any visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) override;
        virtual antlrcpp::Any visitAffectationVaV(ifccParser::AffectationVaVContext *ctx) override;
        virtual antlrcpp::Any visitAffectationVaC(ifccParser::AffectationVaCContext *ctx) override;

        bool isInTable(std::string varName);

    private :
        std::vector<Variable> symbolTable;
};