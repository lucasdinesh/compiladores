%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "hash.h"


	int yylex();
	int yyerror(char *message);
	extern int getLineNumber();
%}

%token KW_CHAR
%token KW_INT
%token KW_FLOAT
%token KW_BOOL

%token KW_IF
%token KW_ELSE
%token KW_WHILE
%token KW_READ
%token KW_PRINT
%token KW_RETURN

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token TK_IDENTIFIER

%token LIT_INT
%token LIT_CHAR
%token LIT_REAL
%token LIT_FALSE
%token LIT_TRUE
%token LIT_STRING

%token TOKEN_ERROR

%left '|' '&' '~'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/' '%'

%%

programa: decl
;

decl: dec decl
    |
    ;

dec:    typos TK_IDENTIFIER ':' literais ';'
         | typos TK_IDENTIFIER '[' LIT_INT ']' declvector ';'
         | typos TK_IDENTIFIER '(' paraml ')' cmd_block
         ;


declvector: ':' literais restovector
            |
            ;

restovector: literais restovector
            |
            ;

param: typos TK_IDENTIFIER
        ;

paraml: param resto_param
        |
        ;

resto_param: ',' param resto_param
            |
            ;

cmd_block: '{' lcmd '}'
;

lcmd: cmd lcmd
    | ';' lcmd
    |
    ;

cmd: TK_IDENTIFIER '=' expr ';'
    | TK_IDENTIFIER '['expr']' '=' expr ';'
    | KW_IF '(' expr ')' resto_if
    | KW_WHILE '(' expr ')' cmd
    | KW_PRINT options_print ';'
    | KW_READ typos TK_IDENTIFIER ';'
    | cmd_block
    | KW_RETURN expr ';'
    ;


 options_print: LIT_STRING
                | typos expr

 resto_if: cmd KW_ELSE cmd
           | cmd
           ;

expr: TK_IDENTIFIER '['expr']'
     | literais
     | TK_IDENTIFIER
     | expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     | expr '%' expr
     | expr '>' expr
     | expr '<' expr
     | expr '&' expr
     | expr '|' expr
     | expr '~' expr
     | expr OPERATOR_LE expr
     | expr OPERATOR_GE expr
     | expr OPERATOR_EQ expr
     | expr OPERATOR_DIF expr
     | '(' expr ')'
     | TK_IDENTIFIER '(' argl ')'
     ;

argl: expr arg_rest
      |
      ;

arg_rest: ',' expr arg_rest
        |
        ;

literais: LIT_INT
         | LIT_CHAR
         | LIT_REAL
         | LIT_FALSE
         | LIT_TRUE
         | LIT_STRING
         ;

typos: KW_CHAR
      | KW_INT
      | KW_FLOAT
      | KW_BOOL
      ;
%%

int yyerror(char *message){
fprintf(stderr, "Syntax error, line = %d.\n", getLineNumber());
exit(3);
}