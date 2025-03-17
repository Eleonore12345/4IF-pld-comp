grammar ifcc;

axiom : prog EOF ;

prog : 'int' 'main' '(' ')' '{' instr '}' ;


instr : TYPE VAR ';' instr # declaration
    | TYPE VAR '=' expr ';' instr # definition
    | VAR '=' expr ';' instr # affectation
    | return_stmt  # return
    ;

expr : VAR # variableSimple
        | CONST # constante
        ;

return_stmt : RETURN CONST ';' ;

RETURN : 'return' ;
TYPE : 'int' ;
CONST : [0-9]+ ;
VAR : [a-zA-Z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);