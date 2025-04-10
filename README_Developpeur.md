# Documentation Développeur

## Gestion des différents scopes

## Intermediate Representation (IR)

## Gestion des différentes fonctions
Afin de gérer certains cas d'erreurs ou de warnings à propos des déclarations et des appels de fonction, nous utilisons une "table de fonctions". Cette table de fonctions est déclarée dans le fichier "FunctionTable.h" et implémentée dans le fichier "FunctionTable.cpp". Elle est utilisée par les trois visiteurs.
### Structure de la table de fonctions
Cette table possède deux attributs : la fonction courante, c'est-à-dire la fonction dans laquelle se trouve les instructions que nous sommes en train de visiter, et un vecteur de "function_identifier", contenant toutes les informations nécessaires de chaque fonction définie et/ou appelée dans le programme à compiler.  
Le type "function_identifier" est une structure de données décrivant une fonction. Ce type possède comme attributs : 
- <ins>functionName</ins> : de type **string** et contenant le nom de la fonction;
- <ins>retourType</ins> : de type **string** et contenant le type du retour de la fonction;
- <ins>nbParams</ins> : de type **int** et contenant le nombre de paramètres de la fonction;
- <ins>def</ins> : de type **bool** et indiquant si la fonction a été définie;
- <ins>rval</ins> : de type **bool** et indiquant si la fonction est utilisée comme valeur d'affectation ou dans une expression;
- <ins>hasReturn</ins> : de type **bool** et indiquant si la fonction contient une instruction return ou non.
### Utilisation
=> Visiteur 1, Visiteur 2, Visiteur 3
=> Chaque appel de fonction blablabla
=> Chaque définition blablabla
=> Warnings et Erreurs
