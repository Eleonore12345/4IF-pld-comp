#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"
#include "FunctionTable.h"

class  IdentifierVisitor : public ifccBaseVisitor {
	public:
                IdentifierVisitor(SymbolTable* symboleTable, FunctionTable * functionTable);
                virtual ~IdentifierVisitor(){};
                virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;
                virtual antlrcpp::Any visitInitDecla(ifccParser::InitDeclaContext *ctx) override;
                virtual antlrcpp::Any visitAffectation(ifccParser::AffectationContext *ctx) override;
                virtual antlrcpp::Any visitVariableSimple(ifccParser::VariableSimpleContext *ctx) override;
                virtual antlrcpp::Any visitOpMultDiv(ifccParser::OpMultDivContext *ctx) override;
                virtual antlrcpp::Any visitConstante(ifccParser::ConstanteContext *ctx) override;
                virtual antlrcpp::Any visitDefFunc(ifccParser::DefFuncContext * ctx) override;
                virtual antlrcpp::Any visitFunctionCall(ifccParser::FunctionCallContext *ctx) override;
                virtual antlrcpp::Any visitNoParam(ifccParser::NoParamContext *ctx) override;
                virtual antlrcpp::Any visitWithParams(ifccParser::WithParamsContext *ctx) override;
                virtual antlrcpp::Any visitNoArg(ifccParser::NoArgContext *ctx) override;
                virtual antlrcpp::Any visitWithArgs(ifccParser::WithArgsContext *ctx) override;
                virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
                virtual antlrcpp::Any visitParentheses(ifccParser::ParenthesesContext *ctx) override;
                virtual antlrcpp::Any visitOpAddSub(ifccParser::OpAddSubContext *ctx) override;
                virtual antlrcpp::Any visitOpUnExpr(ifccParser::OpUnExprContext *ctx) override;
                virtual antlrcpp::Any visitOpUnConst(ifccParser::OpUnConstContext *ctx) override;
                virtual antlrcpp::Any visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx) override;
                virtual antlrcpp::Any visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx) override;
                virtual antlrcpp::Any visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx) override;
                virtual antlrcpp::Any visitOpComp(ifccParser::OpCompContext *ctx) override;
                virtual antlrcpp::Any visitBloc(ifccParser::BlocContext *ctx) override;
                
        private:
                void verifExprPasFctVoid(ifccParser::ExprContext * ctx);
                void addTempVariable();
                SymbolTable* symbolTable;
                FunctionTable * funcTable;
};