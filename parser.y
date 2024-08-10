%{
        #include <stdio.h>
	#include <stdlib.h>
        #include "hash.h"
        #include "ast.h"
        #include "semantic.h"
        #include "tacs.h"


	int yylex();
	int yyerror(char *message);
	extern int getLineNumber();
        AST *root;
        int SemanticErrors;
%}

%union
{
        hash_node *symbol;
        AST *ast;
}


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

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INT
%token<symbol> LIT_CHAR
%token<symbol> LIT_REAL
%token<symbol> LIT_FALSE
%token<symbol> LIT_TRUE
%token<symbol> LIT_STRING

%token TOKEN_ERROR

%left '|' '&' '~'
%left '<' '>' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '+' '-'
%left '*' '/' '%'


%type<ast> programa;
%type<ast> decl;
%type<ast> dec;
%type<ast> declvector;
%type<ast> restovector;
%type<ast> param;
%type<ast> paraml;
%type<ast> resto_param;
%type<ast> cmd_block;
%type<ast> lcmd;
%type<ast> cmd;
%type<ast> options_print;
%type<ast> expr;
%type<ast> literais;
%type<ast> typos;
%type<ast> argl;
%type<ast> argl_rest;

%start programa

%%

programa: decl                   {root=$$;
                                SemanticErrors = checkSemantic(root);
                        }
;

decl: dec decl                          {$$=astCreate(AST_DECL, 0, $1, $2, 0, 0);}
    |                                   {$$=0;}
    ;

dec:    typos TK_IDENTIFIER ':' literais ';'                            {$$= astCreate(AST_DECVAR,$2,$1,$4,0,0);} 
         | typos TK_IDENTIFIER '[' LIT_INT ']' declvector ';'           {$$= astCreate(AST_DECVET,$2,$1,astCreate(AST_SYMBOL, $4, 0, 0, 0, 0),$6,0);} 
         | typos TK_IDENTIFIER '(' paraml ')' cmd_block                 {$$=astCreate(AST_DECFUNC, $2, $1, $4, $6, 0);}
         ;


declvector: ':' literais restovector                                    {$$=astCreate(AST_VECINIT, 0, $2, $3, 0, 0);}
            |                           {$$=0;}
            ;

restovector: literais restovector                                       {$$=astCreate(AST_VECREST, 0, $1, $2, 0, 0);}
            |                           {$$=0;}
            ; 

param: typos TK_IDENTIFIER                      {$$=astCreate(AST_PARAM, $2, $1, 0, 0, 0);}
        ;

paraml: param resto_param                      {$$=astCreate(AST_PARAML, 0, $1, $2, 0, 0);}
        |                               {$$=0;}
        ;

resto_param: ',' param resto_param              {$$=astCreate(AST_PARAMLREST, 0, $2, $3, 0, 0);}
            |                           {$$=0;}
            ;

cmd_block: '{' lcmd '}'                         {$$=astCreate(AST_CMDBLOCK, 0, $2, 0, 0, 0);}
;

lcmd: cmd lcmd                                  {$$=astCreate(AST_LCMDINIT, 0, $1, $2, 0, 0);}                                                      
    |                                           {$$ =0;}
    ;

cmd: TK_IDENTIFIER '=' expr ';'                 {$$= astCreate(AST_ATTR,$1,$3,0,0,0);}  
    | TK_IDENTIFIER '['expr']' '=' expr ';'     {$$= astCreate(AST_VECATTR,$1,$3,$6,0,0);}  
    | KW_IF '(' expr ')' cmd                    {$$= astCreate(AST_IF,0,$3,$5,0,0);}
    | KW_IF '(' expr ')' cmd KW_ELSE cmd        {$$= astCreate(AST_IFELSE,0,$3,$5,$7,0);}
    | KW_WHILE '(' expr ')' cmd                 {$$= astCreate(AST_WHILE,0,$3,$5,0,0);}
    | KW_PRINT options_print ';'                {$$= astCreate(AST_PRINT,0,$2,0,0,0);}
    | KW_READ typos TK_IDENTIFIER ';'           {$$=astCreate(AST_READ, $3, $2, 0, 0, 0);}
    | KW_RETURN expr ';'                        {$$=astCreate(AST_RETURN, 0, $2, 0, 0, 0);}
    | cmd_block                                 {$$=$1;}
    | ';'                                       {$$=astCreate(AST_SEMICOLON, 0, 0, 0, 0, 0);}
    ;


 options_print: LIT_STRING                      {$$=astCreate(AST_SYMBOL, $1, 0, 0, 0, 0);}         
        | typos expr                            {$$=astCreate(AST_PRINTWDECL, 0, $1, $2, 0, 0);}
        ;

expr: TK_IDENTIFIER '['expr']'          {$$= astCreate(AST_VEC_CALL,$1,$3,0,0,0);}            
     | TK_IDENTIFIER                    {$$= astCreate(AST_SYMBOL,$1,0,0,0,0);}                           
     | literais                         {$$=$1;}        
     | expr '+' expr                    {$$= astCreate(AST_ADD,0,$1,$3,0,0);}                        
     | expr '-' expr                    {$$= astCreate(AST_SUB,0,$1,$3,0,0);}                        
     | expr '*' expr                    {$$= astCreate(AST_MULT,0,$1,$3,0,0);}                        
     | expr '/' expr                    {$$= astCreate(AST_DIV,0,$1,$3,0,0);}                        
     | expr '>' expr                    {$$= astCreate(AST_GREATHER,0,$1,$3,0,0);}                        
     | expr '<' expr                    {$$= astCreate(AST_LESS,0,$1,$3,0,0);}                        
     | expr '&' expr                    {$$= astCreate(AST_AND,0,$1,$3,0,0);}                        
     | expr '|' expr                    {$$= astCreate(AST_OR,0,$1,$3,0,0);}                        
     | '~' expr                         {$$= astCreate(AST_NOT,0,$2,0,0,0);}                         
     | expr OPERATOR_LE expr            {$$= astCreate(AST_LE,0,$1,$3,0,0);}
     | expr OPERATOR_GE expr            {$$= astCreate(AST_GE,0,$1,$3,0,0);}
     | expr OPERATOR_EQ expr            {$$= astCreate(AST_EQ,0,$1,$3,0,0);}
     | expr OPERATOR_DIF expr           {$$= astCreate(AST_DIF,0,$1,$3,0,0);}
     | '(' expr ')'                     {$$= astCreate(AST_PAREN,0,$2,0,0,0);}            
     | TK_IDENTIFIER '(' argl ')'       {$$=astCreate(AST_FUNC_CALL, $1, $3, 0, 0, 0);}                      
     ;

argl: expr argl_rest                     {$$=astCreate(AST_ARGL, 0, $1, $2, 0, 0);}
      |                                     {$$=0;}
      ;

argl_rest: ',' expr argl_rest             {$$=astCreate(AST_ARGLREST, 0, $2, $3, 0, 0);}
        |                               {$$=0;}
        ;

literais: LIT_INT               {$$= astCreate(AST_SYMBOL,$1,0,0,0,0);}    
         | LIT_CHAR             {$$= astCreate(AST_SYMBOL,$1,0,0,0,0);}  
         | LIT_REAL             {$$= astCreate(AST_SYMBOL,$1,0,0,0,0);}  
         | LIT_FALSE            {$$= astCreate(AST_SYMBOL,$1,0,0,0,0);}  
         | LIT_TRUE             {$$= astCreate(AST_SYMBOL,$1,0,0,0,0);}  
         ;

typos: KW_CHAR                  {$$=astCreate(AST_TYPECHAR, 0, 0, 0, 0, 0);}
      | KW_INT                  {$$=astCreate(AST_TYPEINT, 0, 0, 0, 0, 0);}
      | KW_FLOAT                {$$=astCreate(AST_TYPEFLOAT, 0, 0, 0, 0, 0);}
      | KW_BOOL                 {$$=astCreate(AST_TYPEBOOL, 0, 0, 0, 0, 0);}
      ;
%%

int yyerror(char *message){
fprintf(stderr, "Syntax error, line = %d.\n", getLineNumber());
exit(3);
}

AST* getAST(){
        return root;
}

void checkSemanticErrors(){
  if(SemanticErrors > 0){
    fprintf(stderr, "\nCompilation finished with %d semantic errors.\n", SemanticErrors);
    exit(4);
  }

    }

  tac* getTACS(){
  return reverseTacList(createTacs(root, 0));
}
