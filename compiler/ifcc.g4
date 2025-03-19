grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' instr* '}' ;


instr : TYPE decla ';' # declaration
    | VAR '=' expr ';' # affectation
    | return_stmt  # return
    ;

decla : initDecla (',' initDecla)* ;

initDecla : VAR ('=' expr)? ;

expr : expr OP=('*'|'/') expr # opMultDiv
    | expr OP=('+'|'-') expr # opAddSub
    | '(' expr ')' # parentheses
    | VAR # variableSimple
    | CONST # constante
    | SYMB_UN expr # symboleUnExpr
    ;

return_stmt : RETURN expr ';' ;

RETURN : 'return' ;
TYPE : 'int' ;
CONST : ([0-9]+ | '\''.'\'') ;
VAR : [a-zA-Z]+ ;
SYMB_UN : ('+' | '-') ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);