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

  // Map répertoriant les fonctions définies avec leur nombre d'arguments
  map<string, int> fonctionsDefined;
  fonctionsDefined["putchar"] = 1;
  fonctionsDefined["getchar"] = 0;
  // Table de symboles pour les variables
  SymbolTable* s = new SymbolTable();
  CFG * c = new CFG();

  IdentifierVisitor i(s,&fonctionsDefined);
  i.visit(tree);

  if (i.getError()) {
    delete s;
    return 1;
  }

  CodeGenVisitor v(s,c,&fonctionsDefined);
  v.visit(tree);
  //c->afficher_CFG();

  //TODO target en argument

  AssemblyX86 a(c,s);
  a.generateAssemblyX86();

  //s->print();

  delete c;
  delete s;
  return 0;
}
