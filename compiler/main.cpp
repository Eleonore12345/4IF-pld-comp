#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccLexer.h"
#include "generated/ifccParser.h"
#include "generated/ifccBaseVisitor.h"

#include "CodeGenVisitor.h"
#include "IdentifierVisitor.h"
#include "AssemblyX86.h"
#include "FunctionTable.h"

using namespace antlr4;
using namespace std;

int main(int argn, const char **argv)
{
  stringstream in; 
  if (argn==2)
  {
     ifstream lecture(argv[1]);
     if( !lecture.good() )
     {
         cerr<<"error: cannot read file: " << argv[1] << endl ;
         exit(1);
     }
     in << lecture.rdbuf();
  }
  else
  {
      cerr << "usage: ifcc path/to/file.c" << endl ;
      exit(1);
  }
  
  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  tree::ParseTree* tree = parser.axiom();

  if(parser.getNumberOfSyntaxErrors() != 0)
  {
      cerr << "error: syntax error during parsing" << endl;
      exit(1);
  }

  FunctionTable* f = new FunctionTable();
  SymbolTable* s = new SymbolTable();

  //Premier visiteur
  IdentifierVisitor i(s,f);
  i.visit(tree);

  //Second visiteur
  CodeGenVisitor v(s,f);
  v.visit(tree);


  //Génération du code assembleur
  vector<CFG*> listeCFG = v.getCfgs();
  AssemblyX86 a(listeCFG,s);
  a.generateAssemblyX86();
  
  //libération de la mémoire
  v.deleteCfgs();
  delete s;
  delete f;
  return 0;
}
