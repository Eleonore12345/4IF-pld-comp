# C compiler with antlr4/c++

## Source Files
- `ifcc.g4` contains the grammar in antlr4 format
- `main.cpp` contains the C++ code to call the antlr4-generated parser
  on the file name provided in the command line.
- `CodeGenVisitor.h` is the visitor used to parse the C input and call IR instructions.
- `IdentifierVisitor.h` is the visitor used to fill in symbol and function tables and scopes.
- `IR.h` is used to stack intermediary representations corresponding to parsed code
- `AssemblyArm64.h` and `Assemblyx86.h` are generating assembly code for Arm64 architecture (resp. x86 architecture)
- `ScopeNode.h` manages scopes for blocs in the control flow
- `SymbolTable.h` and `FunctionTable.h` are used to stack symbols and functions parsed


## Prerequisites
  Before building your  compiler, you should install  antlr using your
  distribution's  package manager, or by following the steps given in [the main README.md](../README_Developpeur.md),  or by  running the  provided shell
  script:   `install-antlr.sh`. 
