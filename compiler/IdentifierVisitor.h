#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"
#include "SymbolTable.h"


class  IdentifierVisitor : public ifccBaseVisitor {
	public:
                IdentifierVisitor(SymbolTable* symboleTable);
                virtual ~IdentifierVisitor(){};
                bool getError();
                virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override;
                virtual antlrcpp::Any visitDeclaration(ifccParser::DeclarationContext *ctx) override;
                virtual antlrcpp::Any visitDefinition(ifccParser::DefinitionContext *ctx) override;
                virtual antlrcpp::Any visitAffectation(ifccParser::AffectationContext *ctx) override;
                virtual antlrcpp::Any visitVariableSimple(ifccParser::VariableSimpleContext *ctx) override;
        private:
                SymbolTable* symTable;
                bool error;
};