grammar ifcc;

axiom : prog EOF ;

prog : defFunc*;

defFunc : typeFunc VAR '(' params ')' '{' instr* '}';

typeFunc : 'void' # typeFunctionVoid
    | TYPE # typeFunctionNoVoid
    ;

instr : decla ';' # declaration
    | expr ';' # expression
    | return_stmt  # return
    ;

decla : TYPE initDecla (',' initDecla)* ;

initDecla : VAR ('=' expr)? ;

expr : VAR '(' args ')' # functionCall
    | OP=('+'|'-'|'!') CONST # opUnConst
    | OP=('+'|'-'|'!') expr # opUnExpr
    | expr OP=('*'|'/'|'%') expr # opMultDiv
    | expr OP=('+'|'-') expr # opAddSub
    | expr OP=('<'|'>'|'==' | '!=') expr #opComp
    | expr '&' expr # opBitwiseAnd
    | expr '^' expr # opBitwiseXor
    | expr '|' expr # opBitwiseOr
    | '(' expr ')' # parentheses
    | VAR '=' expr # affectation
    | VAR # variableSimple
    | CONST # constante
    ;

params : # noParam
    | TYPE VAR (',' TYPE VAR)* # withParams
    ;

args : # noArg
    | expr (',' expr)* # withArgs
    ;
    
return_stmt : RETURN expr? ';' ;

RETURN : 'return' ;
TYPE : 'int' ;
CONST : ([0-9]+ | '\''.'\'') ;
VAR : ([a-zA-Z] | '_') ([a-zA-Z0-9] | '_')* ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);