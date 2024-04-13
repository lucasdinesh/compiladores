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
         | typos TK_IDENTIFIER '(' paraml ')' body
         ;


declvector: ':' literais restovector
            |
            ;

restovector: literais restovector
            |
            ;

paraml: typos TK_IDENTIFIER param
        |
        ;

param: ',' typos TK_IDENTIFIER param
        |
        ;

body: '{' lcmd '}'
;

lcmd: cmd ';' resto_cmd
    |
    ;

resto_cmd: cmd ';' resto_cmd
            |
            ;

cmd: TK_IDENTIFIER '=' expr
    | KW_IF '(' expr ')' resto_if
    | KW_WHILE '(' expr ')' body
    ;

 resto_if: body KW_ELSE body
           | body
           ;

expr: literais
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

int yyerror ()
{
fprintf(stderr, "Syntax error, line = %d.\n", getLineNumber());
exit(3);
}