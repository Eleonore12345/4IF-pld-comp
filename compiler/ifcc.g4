grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' expr '}' ;
expr : return_stmt #return
| TYPE VAR ';' #dec
| TYPE VAR '=' CONST ';' #affDecConst
| TYPE VAR '=' VAR ';' #affDecVar
| VAR '=' VAR ';' #affVar
| VAR '=' CONST ';' #affConst
;

return_stmt: RETURN CONST ';' ;

RETURN : 'return' ;
CONST : [0-9]+ ;
TYPE : 'int' ;
VAR : [a-zA-Z]+ ; 
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
