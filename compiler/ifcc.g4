grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' instr* '}' ;

instr : decla ';' # declaration
    | VAR '=' expr ';' # affectation
    | return_stmt  # return
    ;

decla : TYPE initDecla (',' initDecla)* ;

initDecla : VAR ('=' expr)? ;

expr : OP=('+'|'-') CONST # opUnConst
    | OP=('+'|'-') expr # opUnExpr
    | expr OP=('*'|'/'|'%') expr # opMultDiv
    | expr OP=('+'|'-') expr # opAddSub
    | expr OP=('&'|'|'|'^') expr # opBitwise
    | '(' expr ')' # parentheses
    | VAR # variableSimple
    | CONST # constante
    ;

return_stmt : RETURN expr ';' ;

RETURN : 'return' ;
TYPE : 'int' ;
CONST : ([0-9]+ | '\''.'\'') ;
VAR : [a-zA-Z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);