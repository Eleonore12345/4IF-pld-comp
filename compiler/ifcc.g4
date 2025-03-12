grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' expr '}' ;


expr : TYPE VAR ';' expr # declaration
    | TYPE VAR '=' VAR ';' expr # definitionV
    | TYPE VAR '=' CONST ';' expr # definitionC
    | VAR '=' VAR ';' expr # affectationV
    | VAR '=' CONST ';' expr # affectationC
    | return_stmt  # return
    ;

return_stmt : RETURN CONST ';' ;

RETURN : 'return' ;
TYPE : 'int' ;
CONST : [0-9]+ ;
VAR : [a-zA-Z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);