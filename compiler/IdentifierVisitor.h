#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"

class  IdentifierVisitor : public ifccBaseVisitor {
	public:
                IdentifierVisitor(SymbolTable* symboleTable, map<string,int> * fonctionsDef);
                virtual ~IdentifierVisitor(){};
                bool getError();
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

        private:
                SymbolTable* symTable;
                map<string,int> * fonctionsDefined;
                bool error;
};