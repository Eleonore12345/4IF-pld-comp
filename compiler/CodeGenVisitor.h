#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "CodeGenSymbolTable.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        CodeGenVisitor(std::vector<Variable> & s):symbolTable(s){};

        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitDeclarationV(ifccParser::DeclarationVContext *ctx) override;
        //virtual antlrcpp::Any visitDeclarationVaV(ifccParser::DeclarationVaVContext *ctx) override;
        virtual antlrcpp::Any visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) override;
        //virtual antlrcpp::Any visitAffectationVaV(ifccParser::AffectationVaVContext *ctx) override;
        //virtual antlrcpp::Any visitAffectationVaC(ifccParser::AffectationVacContext *ctx) override;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;

        int getIndex(std::string varName);

        private:
        std::vector<Variable> symbolTable;
};

