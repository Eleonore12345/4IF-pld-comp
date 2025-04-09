#ifndef IR_H
#define IR_H

#include <vector>
#include <string>
#include <iostream>
#include <initializer_list>
#include <map>

// Declarations from the parser -- replace with your own
#include "SymbolTable.h"

class BasicBlock;
class CFG;
class DefFonction;

typedef enum {
	INT,
} Type;

//! The class for one 3-address instruction
class IRInstr {
 
   public:
	/*les instructions génériques, les noms proposés sont ceux de base, il est tout à fait possible de les modifier (attention à pas modifier l'ordre pour le switch),
	Précisez les paramètres et l'ordre pour éviter les embrouilles*/
	typedef enum {
		ldconst, // a=4 {a,4}
		ldconstneg, //a=-2 {a,2}
		copy, // a=b {a,b}
		negexpr, // a=-b {a,b}
		notexpr, //a=!b
		notconst, //a=!const
		add, // a=b+c {a,b,c}
		sub, // a=b-c {a,b,c}
		mul, // a=b*c {a,b,c}
		div, // a=b/c {a,b,c}
		mod, // a=b%c {a,b,c}
		and_bit, 
		or_bit,
		xor_bit,		
		inf,
		sup,
		eq,
		diff,
		retour,  // return expr {expr}
		functionCall, // a = function(arg1, arg2, ...) {a, function, arg[]}
		functionDef, // int/void function(int arg1, int arg2, ...) { instr* }
		enter_bloc,
		leave_bloc,
	} Operation;


	/**  constructor */
	IRInstr(BasicBlock* bb_, Operation op, Type t, vector<string> params);
	void afficher_instr();
	/** Actual code generation */
	Operation getOperation() { return op; }
	vector <string> getParams() { return params;}

 private:
	BasicBlock* bb; /**< The BB this instruction belongs to, which provides a pointer to the CFG this instruction belong to */
	Operation op;
	Type t;
	vector<string> params; /**< For 3-op instrs: d, x, y; for ldconst: d, c;  For call: label, d, params;  for wmem and rmem: choose yourself */
	// if you subclass IRInstr, each IRInstr subclass has its parameters and the previous (very important) comment becomes useless: it would be a better design. 
};






/**  The class for a basic block */

/* A few important comments.
	 IRInstr has no jump instructions.
	 cmp_* instructions behaves as an arithmetic two-operand instruction (add or mult),
	  returning a boolean value (as an int)

	 Assembly jumps are generated as follows:
	 BasicBlock::gen_asm() first calls IRInstr::gen_asm() on all its instructions, and then 
		    if  exit_true  is a  nullptr, 
            the epilogue is generated
        else if exit_false is a nullptr, 
          an unconditional jmp to the exit_true branch is generated
				else (we have two successors, hence a branch)
          an instruction comparing the value of test_var_name to true is generated,
					followed by a conditional branch to the exit_false branch,
					followed by an unconditional branch to the exit_true branch
	 The attribute test_var_name itself is defined when converting 
  the if, while, etc of the AST  to IR.

Possible optimization:
     a cmp_* comparison instructions, if it is the last instruction of its block, 
       generates an actual assembly comparison 
       followed by a conditional jump to the exit_false branch
*/

class BasicBlock {
 public:
	BasicBlock(CFG* cfg, string entry_label);
	void afficher_bloc();
	void add_IRInstr(IRInstr::Operation op, Type t, vector<string> params);

	BasicBlock* exit_true;  /**< pointer to the next basic block, true branch. If nullptr, return from procedure */ 
	BasicBlock* exit_false; /**< pointer to the next basic block, false branch. If null_ptr, the basic block ends with an unconditional jump */

	string label; /**< label of the BB, also will be the label in the generated code */
	CFG* cfg; /** < the CFG where this block belongs */
	vector<IRInstr*> instrs; /** < the instructions themselves. */

 protected:
 
};




/** The class for the control flow graph, also includes the symbol table */

/* A few important comments:
	 The entry block is the one with the same label as the AST function name.
	   (it could be the first of bbs, or it could be defined by an attribute value)
	 The exit block is the one with both exit pointers equal to nullptr.
     (again it could be identified in a more explicit way)

 */
class CFG {
 public:
	CFG(DefFonction* ast);

	CFG(){
		nextBBnumber = 0;
	};

	DefFonction* ast; /**< The AST this CFG comes from */
	
	void add_bb(BasicBlock* bb); 

	void afficher_CFG();
	// basic block management
	string new_BB_name(string name);
	BasicBlock* current_bb;
	vector <BasicBlock*> get_bbs() { return bbs; }

 protected:

	int nextBBnumber; /**< just for naming */
	
	vector <BasicBlock*> bbs; /**< all the basic blocks of this CFG*/

};


#endif