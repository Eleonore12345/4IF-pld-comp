grammar ifcc;

axiom : prog EOF ;

prog : TYPE 'main' '(' ')' '{' expr '}' ;

expr : TYPE VAR ';' expr #declarationV
     | TYPE VAR '=' VAR ';' expr #declarationVaV
     | TYPE VAR '=' CONST ';' expr #declarationVaC
     | VAR '=' VAR ';' expr #affectationVaV
     | VAR '=' CONST ';' expr #affectationVaC
     | return_stmt #return;

return_stmt: RETURN CONST ';' ;

RETURN : 'return' ;
TYPE : 'int' ;
CONST : [0-9]+ ;
VAR : [a-z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
