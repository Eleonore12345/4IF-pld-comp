#pragma once

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

class  AffectVisitor : public ifccBaseVisitor {
	public:

        virtual antlrcpp::Any visitAxiom(ifccParser::AxiomContext *ctx) override ;

        virtual antlrcpp::Any AffectVisitor::visitProg(ifccParser::ProgContext *ctx) override ;
        
        virtual antlrcpp::Any visitDec(ifccParser::DecContext *ctx) override ;
        
        virtual antlrcpp::Any visitAffDecConst(ifccParser::AffDecConstContext *ctx) override ;
        
        virtual antlrcpp::Any visitAffDecVar(ifccParser::AffDecVarContext *ctx) override ;
        
        virtual antlrcpp::Any visitAffVar(ifccParser::AffVarContext *ctx) override ;
        
        virtual antlrcpp::Any visitAffConst(ifccParser::AffConstContext *ctx) override ;

};

