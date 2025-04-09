#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"
#include "FunctionTable.h"
#include "IR.h"



class  CodeGenVisitor : public ifccBaseVisitor {
	public:
                CodeGenVisitor(SymbolTable * s, FunctionTable * functionTable);
                virtual ~CodeGenVisitor(){};

                virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
                virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
                virtual antlrcpp::Any visitParentheses(ifccParser::ParenthesesContext *ctx) override;
                virtual antlrcpp::Any visitConstante(ifccParser::ConstanteContext *ctx) override;
                virtual antlrcpp::Any visitOpAddSub(ifccParser::OpAddSubContext *ctx) override;
                virtual antlrcpp::Any visitVariableSimple(ifccParser::VariableSimpleContext *ctx) override;
                virtual antlrcpp::Any visitOpMultDiv(ifccParser::OpMultDivContext *ctx) override;
                virtual antlrcpp::Any visitOpUnConst(ifccParser::OpUnConstContext *ctx) override;
                virtual antlrcpp::Any visitOpUnExpr(ifccParser::OpUnExprContext *ctx) override;
                virtual antlrcpp::Any visitAffectation(ifccParser::AffectationContext *ctx) override;
                virtual antlrcpp::Any visitInitDecla(ifccParser::InitDeclaContext * ctx) override;
                virtual antlrcpp::Any visitOpBitwiseAnd(ifccParser::OpBitwiseAndContext *ctx) override;
                virtual antlrcpp::Any visitOpBitwiseXor(ifccParser::OpBitwiseXorContext *ctx) override;
                virtual antlrcpp::Any visitOpBitwiseOr(ifccParser::OpBitwiseOrContext *ctx) override;
                virtual antlrcpp::Any visitOpComp(ifccParser::OpCompContext *ctx) override;
                virtual antlrcpp::Any visitExpression(ifccParser::ExpressionContext *ctx) override;
                virtual antlrcpp::Any visitDefFunc(ifccParser::DefFuncContext * ctx) override;
                virtual antlrcpp::Any visitNoParam(ifccParser::NoParamContext *ctx) override;
                virtual antlrcpp::Any visitWithParams(ifccParser::WithParamsContext *ctx) override;
                virtual antlrcpp::Any visitFunctionCall(ifccParser::FunctionCallContext *ctx) override;
                virtual antlrcpp::Any visitNoArg(ifccParser::NoArgContext *ctx) override;
                virtual antlrcpp::Any visitWithArgs(ifccParser::WithArgsContext *ctx) override;
                virtual antlrcpp::Any visitBloc(ifccParser::BlocContext *ctx) override;
                virtual antlrcpp::Any visitIf_stmt(ifccParser::If_stmtContext *ctx) override;
                virtual antlrcpp::Any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
                    
                    
                
                void VariableOrConstante(string name1, string name2);
                vector<CFG*> getCfgs(){return cfgs;};
                void deleteCfgs();
        private :
                SymbolTable * symbolTable;
                FunctionTable * funcTable;
                vector<CFG*> cfgs;
                CFG * currentCfg;
};

