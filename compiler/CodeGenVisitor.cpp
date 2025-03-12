#include "CodeGenVisitor.h"
#include <map>
#include <iostream>

std::map<std::string, int> CodeGenVisitor::symbolTable;
int CodeGenVisitor::stackOffset = 0;

// Fonction pour ajouter une nouvelle variable à la table des symboles
antlrcpp::Any CodeGenVisitor::visitDeclaration(ifccParser::DeclarationContext *ctx)
{
    std::string varName = ctx->VAR()->getText();

    // Décrémenter l'offset de 4 pour allouer de la place sur la pile pour chaque variable (ARM64 mais 32 bits)
    stackOffset -= 4;
    symbolTable[varName] = stackOffset;

    // Si la déclaration inclut une initialisation (ex : int a = 2)
    if (ctx->expr())
    {
        int value = std::stoi(ctx->expr()->getText());
        std::cout << "    mov x0, #" << value << std::endl;                    // 将值加载到 x0 寄存器
        std::cout << "    str x0, [x29, #" << stackOffset << "]" << std::endl; // 将值存储到栈中
    }

    return 0;
}

// Fonction pour traiter une affectation (ex : a = 3; ou b = a;)
antlrcpp::Any CodeGenVisitor::visitAssignment(ifccParser::AssignmentContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    int value = std::stoi(ctx->expr()->getText());

    // Générer le code assembleur pour affecter la valeur à la variable
    std::cout << "    mov x0, #" << value << std::endl;                             // 将值加载到 x0 寄存器
    std::cout << "    str x0, [x29, #" << symbolTable[varName] << "]" << std::endl; // 将值存储到栈中
    return 0;
}

// Fonction pour générer le code du return (ex : return a;)
antlrcpp::Any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    // Vérifier si l'expression est une constante ou une variable
    if (ctx->expr()->CONST())
    {
        // Si c'est une constante, utiliser `stoi` pour récupérer sa valeur
        int retval = std::stoi(ctx->expr()->CONST()->getText());
        std::cout << "    mov x0, #" << retval << std::endl; // 将返回值加载到 x0 寄存器
    }
    else if (ctx->expr()->VAR())
    {
        // Si c'est une variable, récupérer l'offset dans la table des symboles
        std::string varName = ctx->expr()->VAR()->getText();
        int offset = symbolTable[varName];
        std::cout << "    ldr x0, [x29, #" << offset << "]" << std::endl; // 从栈中加载变量值到 x0
    }

    return 0;
}

// Fonction pour gérer les expressions (constantes et variables)
antlrcpp::Any CodeGenVisitor::visitExpr(ifccParser::ExprContext *ctx)
{
    if (ctx->CONST())
    {
        // Si c'est une constante, on la charge directement dans x0
        int value = std::stoi(ctx->CONST()->getText());
        std::cout << "    mov x0, #" << value << std::endl;
    }
    else if (ctx->VAR())
    {
        // Si c'est une variable, on charge sa valeur depuis la pile
        std::string varName = ctx->VAR()->getText();
        std::cout << "    ldr x0, [x29, #" << symbolTable[varName] << "]" << std::endl; // 从栈中加载变量值到 x0
    }
    return 0;
}

// Générer le code pour le programme entier
antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    std::cout << ".global _main\n";
    std::cout << "_main:\n";

    // Prologue
    std::cout << "    stp x29, x30, [sp, #-16]!  // Save x29 (frame pointer) and x30 (link register)\n";
    std::cout << "    mov x29, sp  // Set up frame pointer\n";

    // Visite les déclarations et affectations
    for (auto stmt : ctx->stmt())
    {
        this->visit(stmt);
    }

    // Retour
    this->visit(ctx->return_stmt());

    // Epilogue
    std::cout << "    ldp x29, x30, [sp], #16  // Restore x29 and x30\n";
    std::cout << "    ret  // Return\n";

    return 0;
}
