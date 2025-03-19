#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
                CodeGenVisitor(SymbolTable * s);
                virtual ~CodeGenVisitor(){};

                virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
                virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
                virtual antlrcpp::Any visitParentheses(ifccParser::ParenthesesContext *ctx) override;
                virtual antlrcpp::Any visitConstante(ifccParser::ConstanteContext *ctx) override;
                virtual antlrcpp::Any visitOpAddSub(ifccParser::OpAddSubContext *ctx) override;
                virtual antlrcpp::Any visitVariableSimple(ifccParser::VariableSimpleContext *ctx) override;
                virtual antlrcpp::Any visitOpMultDiv(ifccParser::OpMultDivContext *ctx) override;
                virtual antlrcpp::Any visitAffectation(ifccParser::AffectationContext *ctx) override;
                virtual antlrcpp::Any visitInitDecla(ifccParser::InitDeclaContext * ctx) override;

        private :
                SymbolTable * symbolTable;
};

