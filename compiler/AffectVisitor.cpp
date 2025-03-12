#include "AffectVisitor.h"

virtual antlrcpp::Any AffectVisitor::visitAxiom(ifccParser::AxiomContext *ctx) override {
    cout << "a" << endl;
}

virtual antlrcpp::Any AffectVisitor::visitProg(ifccParser::ProgContext *ctx) override {
    cout << "b" << endl;
}

antlrcpp::Any AffectVisitor::visitDec(ifccParser::DecContext *ctx) override {
        cout << "c"<< endl;
}

antlrcpp::Any AffectVisitor::visitAffDecConst(ifccParser::AffDecConstContext *ctx) override {
    cout << "d" << endl;
}

antlrcpp::Any AffectVisitor::visitAffDecVar(ifccParser::AffDecVarContext *ctx) override {
    cout << "e" << endl;
}

antlrcpp::Any AffectVisitor::visitAffVar(ifccParser::AffVarContext *ctx) override {
    cout <<"f" << endl;
}

antlrcpp::Any AffectVisitor::(ifccParser::AffConstContext *ctx) override {
    cout << "g" << endl;
}