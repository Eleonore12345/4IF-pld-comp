#include "AffectVisitor.h"

using namespace std;

antlrcpp::Any AffectVisitor::visitDec(ifccParser::DecContext *ctx) {
        cout << "c" << endl;
        return 0;

}

antlrcpp::Any AffectVisitor::visitAffDecConst(ifccParser::AffDecConstContext *ctx) {
    cout << "d" << endl;
    return 0;

}

antlrcpp::Any AffectVisitor::visitAffDecVar(ifccParser::AffDecVarContext *ctx) {
    cout << "e" << endl;
    return 0;

}

antlrcpp::Any AffectVisitor::visitAffVar(ifccParser::AffVarContext *ctx) {
    cout <<"f" << endl;
    return 0;

}

antlrcpp::Any AffectVisitor::visitAffConst(ifccParser::AffConstContext *ctx) {
    cout << "g" << endl;
    return 0;
}
