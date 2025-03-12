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
                virtual antlrcpp::Any visitDefinitionV(ifccParser::DefinitionVContext *ctx) override;
                virtual antlrcpp::Any visitDefinitionC(ifccParser::DefinitionCContext *ctx) override;
                virtual antlrcpp::Any visitAffectationV(ifccParser::AffectationVContext *ctx) override;
                virtual antlrcpp::Any visitAffectationC(ifccParser::AffectationCContext *ctx) override;
        private:
                SymbolTable* symTable;
                bool error;

};