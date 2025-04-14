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
#include "AssemblyArm64.h"
#include "FunctionTable.h"

using namespace antlr4;
using namespace std;

int main(int argc, const char **argv)
{

  string target = "x86";
  string inputFile = "";
  stringstream in;

  for (int i = 1; i < argc; i++)
  {
    string arg = argv[i];

    if (arg == "-target")
    {
      if (i + 1 < argc)
      {
        string val = argv[++i];
        if (val == "x86" || val == "arm")
        {
          target = val;
        }
        else
        {
          cerr << "Erreur, valeur invalide pour -target. Valeurs attendues : x86 ou arm " << endl;
          exit(1);
        }
      }
      else
      {
        cerr << "Erreur l'option -target nécessite une valeur. Valeurs attendues : x86 ou arm " << endl;
        exit(1);
      }
    }
    else if (inputFile.empty())
    {
      inputFile = arg;
    }
  }

  if (target.empty())
  {
    cerr << "usage: ifcc [-target option] path/to/file.c" << endl;
    exit(1);
  }

  if (inputFile.empty())
  {
    cerr << "usage: ifcc [-target option] path/to/file.c" << endl;
    exit(1);
  }

  ifstream lecture(inputFile);
  if (!lecture.good())
  {
    cerr << "error: cannot read file: " << inputFile << endl;
    exit(1);
  }
  in << lecture.rdbuf();

  ANTLRInputStream input(in.str());

  ifccLexer lexer(&input);
  CommonTokenStream tokens(&lexer);

  tokens.fill();

  ifccParser parser(&tokens);
  tree::ParseTree *tree = parser.axiom();

  if (parser.getNumberOfSyntaxErrors() != 0)
  {
    cerr << "error: syntax error during parsing" << endl;
    exit(1);
  }

  FunctionTable *f = new FunctionTable();
  SymbolTable *s = new SymbolTable();

  IdentifierVisitor i(s, f);
  i.visit(tree);

  CodeGenVisitor v(s, f);
  v.visit(tree);
  vector<CFG *> listeCFG = v.getCfgs();
  if (target == "x86")
  {
    AssemblyX86 a(listeCFG, s);
    a.generateAssemblyX86();
  }
  else if (target == "arm")
  {
    AssemblyArm64 a(listeCFG, s);
    a.generateAssemblyArm64();
  }
  else
  {
    cerr << "Erreur, valeur invalide pour -target. Valeurs attendues : x86 ou arm " << endl;
    exit(1);
  }
  v.deleteCfgs();
  delete s;
  delete f;
  return 0;
}