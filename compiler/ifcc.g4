grammar ifcc;

axiom : prog EOF ;

prog : TYPE 'main' '(' ')' '{' expr '}' ;

expr 
    : return_stmt #return 
    | TYPE VAR '=' CONST ';' expr #declarationVaC
    | TYPE VAR '=' VAR ';' expr #declarationVaV
    | TYPE VAR ';' expr #declarationV
    | VAR '=' VAR ';' expr #affectationVaV
    | VAR '=' CONST ';' expr #affectationVaC
;

return_stmt: RETURN CONST ';' ;

RETURN : 'return' ;
CONST : [0-9]+ ;
TYPE : 'int' ;
VAR : [a-z]+;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
