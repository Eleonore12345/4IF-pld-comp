#include "IR.h"
#include <iostream>

using namespace std;

CFG::CFG(DefFonction* ast){
    this->ast = ast;
    nextBBnumber = 1;
}

BasicBlock::BasicBlock(CFG* cfg, string entry_label){
    this->cfg = cfg;
    this->label = entry_label;
    exit_true = nullptr;
    exit_false = nullptr;
}

IRInstr::IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params){
    this->bb = bb_;
    this->op = op;
    this->t = t;
    this->params = params;
}

string CFG::new_BB_name(string name){
    return "." + name + to_string(nextBBnumber++);
}

void CFG::add_bb(BasicBlock* bb){
    bbs.push_back(bb);
}

void BasicBlock::add_IRInstr(IRInstr::Operation op, Type t, vector<string> params){
    IRInstr* instr = new IRInstr(this, op, t, params);
    instrs.push_back(instr);
}

void BasicBlock::afficher_bloc(){
    cout << "Basic Block : " << label << endl;
    for (IRInstr* instr : instrs){
        instr->afficher_instr();
    }
}   

void IRInstr::afficher_instr(){
    cout << "Instruction : " << op << endl; 
    cout << "Params : ";
    for (string param : params){
        cout << param << ";";
    }
    cout << endl;
}   

void CFG::afficher_CFG(){
    for (BasicBlock* bb : bbs){
        bb->afficher_bloc();
    }
}