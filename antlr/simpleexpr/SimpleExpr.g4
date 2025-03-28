grammar SimpleExpr;

prog: stat* EOF ; // 一个程序包含多个语句

stat: expr ';' // 一个语句包含一个表达式和一个分号
        | ID '=' expr ';' // 赋值语句
        | 'print' expr ';' // 打印语句
        ;

expr: expr ('*'|'/') expr // 表达式可以是乘除
        | expr ('+'|'-'|) expr // 表达式可以是加减，优先级低于乘除
        | '(' expr ')' // 表达式可以是括号
        | ID // 表达式可以是一个变量
        | INT // 表达式可以是一个整数
        | FLOAT // 表达式可以是一个浮点数
        ;


SEMI : ';' ;
ASSIGN : '=' ;
PRINT : 'print' ;
MUL : '*' ;
DIV : '/' ;
ADD : '+' ;
SUB : '-' ;
LPAREN : '(' ;
RPAREN : ')' ;

ID:('_'|[a-zA-Z])('_'|[a-zA-Z0-9])*; // 变量名可以是字母或下划线开头，后面可以是字母、数字或下划线
INT:'0' |('+'|'-')? [1-9][0-9]*; // 整数
FLOAT: INT '.'DIGIT* | '.' DIGIT+; // 浮点数
DOCS_COMMENT: '/**' .*? '*/' -> skip;
SL_COMMENT: '//' .*? '\n' -> skip;
ML_COMMENT: '/*' .*? '*/' -> skip;
WS: [ \t\r\n]+ -> skip ; // 忽略空白符


fragment LETTER: [a-zA-Z] ; // 字母
fragment DIGIT: [0-9] ; // 数字
fragment WORD: '_' | LETTER | DIGIT ; // 单词
