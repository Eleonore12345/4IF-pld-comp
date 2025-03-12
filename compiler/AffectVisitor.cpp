#include "AffectVisitor.h"

antlrcpp::Any AffectVisitor::visitAxiom(ifccParser::AxiomContext *ctx) {
    cout << "a" << endl;
}

antlrcpp::Any AffectVisitor::visitProg(ifccParser::ProgContext *ctx) {
    cout << "b" << endl;
}

antlrcpp::Any AffectVisitor::visitDec(ifccParser::DecContext *ctx) {
        cout << "c"<< endl;
}

antlrcpp::Any AffectVisitor::visitAffDecConst(ifccParser::AffDecConstContext *ctx) {
    cout << "d" << endl;
}

antlrcpp::Any AffectVisitor::visitAffDecVar(ifccParser::AffDecVarContext *ctx) {
    cout << "e" << endl;
}

antlrcpp::Any AffectVisitor::visitAffVar(ifccParser::AffVarContext *ctx) {
    cout <<"f" << endl;
}

antlrcpp::Any AffectVisitor::(ifccParser::AffConstContext *ctx) {
    cout << "g" << endl;
}

antlrcpp::Any AffectVisitor::(ifccParser::Return_stmtContext *ctx) {
    cout << "h" << endl;
}
