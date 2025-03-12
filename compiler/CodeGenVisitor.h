#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDec(ifccParser::DecContext *ctx) override ;
        virtual antlrcpp::Any visitAffDecConst(ifccParser::AffDecConstContext *ctx) override ;
        virtual antlrcpp::Any visitAffDecVar(ifccParser::AffDecVarContext *ctx) override ;
        virtual antlrcpp::Any visitAffVar(ifccParser::AffVarContext *ctx) override ;
        virtual antlrcpp::Any visitAffConst(ifccParser::AffConstContext *ctx) override ;
};

