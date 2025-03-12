#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "CodeGenSymboleTable.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
                CodeGenVisitor(std::vector<Variable> & s):symboleTable(s){};

                virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
                virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
                virtual antlrcpp::Any visitDeclarationVaC(ifccParser::DeclarationVaCContext *ctx) override;
                virtual antlrcpp::Any visitDeclarationVaV(ifccParser::DeclarationVaVContext * ctx) override;
                virtual antlrcpp::Any visitDeclarationV(ifccParser::DeclarationVContext *ctx) override;
                virtual antlrcpp::Any visitAffectationVaC(ifccParser::AffectationVaCContext *ctx) override;

                int getIndex(std::string varName);
                //virtual antlrcpp::Any visitDeclarationVaC(ifccParser::visit)

        private :
                std::vector<Variable> symboleTable;
};

