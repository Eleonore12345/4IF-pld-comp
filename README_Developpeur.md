<h1 align="center">C - Compilation project</h1>

[![C language logo][c-logo]](https://en.cppreference.com/w/c/language)

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table des matières</summary>
  <ol>
    <li>
      <a href="#gestion-des-différents-scopes">Gestion des différents scopes</a>
    </li>
    <li>
      <a href="#intermediate-representation">Intermediate Representation</a>
    </li>
    <li>
      <a href="#table-de-fonctions">Table de fonctions</a>
      <ul>
        <li><a href="#structure">Structure</a></li>
        <li><a href="#utilisation">Utilisation</a></li>
      </ul>
    </li>
  </ol>
</details>

## Gestion des différents scopes

## Intermediate Representation (IR)

## Table de fonctions
Afin de gérer certains cas d'erreurs ou de warnings à propos des déclarations et des appels de fonction, nous utilisons une "table de fonctions". Cette table de fonctions est déclarée dans le fichier [FunctionTable.h][functiontable-h] et implémentée dans le fichier [FunctionTable.cpp][functiontable-cpp]. Elle est utilisée par les trois visiteurs.
### Structure
Cette table possède deux attributs : la fonction courante, c'est-à-dire la fonction dans laquelle se trouve les instructions que nous sommes en train de visiter, et un vecteur de "function_identifier", contenant toutes les informations nécessaires de chaque fonction définie et/ou appelée dans le programme à compiler.  
Le type "function_identifier" est une structure de données décrivant une fonction. Ainsi, dès qu'une information ou une vérification est nécessaire sur la fonction courante lors des visites, nous pouvons nous référer à cet attribut. Ce type possède comme attributs : 
- <ins>functionName</ins> : de type **string** et contenant le nom de la fonction;
- <ins>retourType</ins> : de type **string** et contenant le type du retour de la fonction;
- <ins>nbParams</ins> : de type **int** et contenant le nombre de paramètres de la fonction;
- <ins>def</ins> : de type **bool** et indiquant si la fonction a été définie;
- <ins>rval</ins> : de type **bool** et indiquant si la fonction est utilisée comme valeur d'affectation ou dans une expression;
- <ins>hasReturn</ins> : de type **bool** et indiquant si la fonction contient une instruction return ou non.
### Utilisation
Lors de la première visite par [IdentifierVisitor][identifier-visitor-cpp], la table de fonctions est utilisée pour trouver les erreurs : 
- Dans la méthode `visitFunctionCall`, nous ajoutons la fonction à la table des fonctions si elle n'est pas encore dedans en initialisant l'attribut "def" à false. Un warning est alors envoyé : `implicit declaration of function fonction`.
- Dans la méthode `visitDefFunc`, nous changeons la fonction courante pour le nom de la fonction dans laquelle nous entrons. Si la fonction n'est pas présente dans la table, nous l'ajoutons. Si elle est présente et définie, nous renvoyons l'erreur `Function <function> already defined`. Si elle est seulement présente (cas de la déclaration implicite), nous la mettons à "définie" et nous renseignons le type du retour.
- Dans la méthode `visitReturn_stmt`, nous vérifions le type du retour de la fonction courante. S'il est à void et que nous renvoyons une expression alors un warning est envoyé : `return with a value, in function returning void : <function>`.
- Dans la méthode `verifExprPasFctVoid`, si l'expression visitée est un appel de fonction alors plusieurs cas se posent. Si la fonction n'est pas encore déclarée, alors c'est une déclaration implicite : ajout de la fonction dans la table de fonctions (avec comme attribut def = false). Si la fonction est déclarée, nous vérifions le type de retour : s'il est à void alors nous renvoyons l'erreur `void value not ignored as it ought to be`. Dans tous les cas, la fonction voit son attribut rval changée à true.
- A la fin de la première visite, les méthodes `checkIfEachFuncDefined` et `checkRvalFuncReturnType` sont appelées et vérifient respectivement si toutes les fonctions appelées sont définies (renvoie d'erreur dans le cas contraire `undefined reference to <function>`) et si une fonction avec un retour de type void est utilisée comme valeur d'affectation ou dans une expression (renvoie un warning `conflicting types for '<function>'; have ‘void()’`).  

Lors de la seconde visite par [CodeGenVisitor][codegenvisitor-cpp], la table de fonctions est utilisée pour :
- Changer de fonction courante lorsque nous entrons dans une nouvelle définition de fonction (méthode `visitDefFunc`)
- Vérifier si la fonction a un return (sinon on fait un simple return vide dans la méthode `visitDefFunc`)
- Vérifier le nombre d'arguments d'un appel de fonction (méthode `visitFunctionCall`) sinon erreur : `too few/many arguments to function '<function>'`

[c-logo]: documents/c_icon.png

[functiontable-h]: compiler/FunctionTable.h
[functiontable-cpp]: compiler/FunctionTable.cpp
[identifier-visitor-cpp]: compiler/IdentifierVisitor.cpp
[codegenvisitor-cpp]: compiler/CodeGenVisitor.cpp
