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
                virtual antlrcpp::Any visitOpUnaire(ifccParser::OpUnaireContext *ctx) override;
                virtual antlrcpp::Any visitDefinition(ifccParser::DefinitionContext *ctx) override;
                virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;
                virtual antlrcpp::Any visitAffectation(ifccParser::AffectationContext *ctx) override;

        private :
                SymbolTable * symbolTable;
};

