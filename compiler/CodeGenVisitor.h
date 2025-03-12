#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
                CodeGenVisitor(SymbolTable * s):symbolTable(s){};

                virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
                virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
                virtual antlrcpp::Any visitDefinitionC(ifccParser::DefinitionCContext *ctx) override;
                virtual antlrcpp::Any visitDefinitionV(ifccParser::DefinitionVContext * ctx) override;
                virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;
                virtual antlrcpp::Any visitAffectationC(ifccParser::AffectationCContext *ctx) override;
                virtual antlrcpp::Any visitAffectationV(ifccParser::AffectationVContext *ctx) override;

        private :
                SymbolTable * symbolTable;
};

