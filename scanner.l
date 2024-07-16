%{

	#include "hash.h"
	#include "ast.h"
	#include "y.tab.h"

int running = 1;
int lineNumber = 1;
%}

%x COMMENT

%%

[-,;{}+:\[\]\(\)\*/\%<>&|~=]        {return yytext[0];}
"//".*
"\n"                                {++lineNumber;}
[ \t]
"/*"                                { BEGIN(COMMENT);}
"bool"                              { return KW_BOOL;}
"int"                               { return KW_INT;}
"char"                              { return KW_CHAR;}
"float"                             { return KW_FLOAT;}
"if"                                { return KW_IF;}
"else"                              { return KW_ELSE;}
"while"                             { return KW_WHILE;}
"read"                              { return KW_READ;}
"return"                            { return KW_RETURN;}
"print"                            { return KW_PRINT;}

"<="                                { return OPERATOR_LE;}
">="                                { return OPERATOR_GE;}
"=="                                { return OPERATOR_EQ;}
"!="                                { return OPERATOR_DIF;}

true                                {yylval.symbol = hashInsert(LIT_TRUE, yytext); return LIT_TRUE ;}
false                               {yylval.symbol = hashInsert(LIT_FALSE, yytext); return LIT_FALSE ;}

[a-zA-Z_][a-zA-Z0-9_]*              {yylval.symbol = hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER;}

'.'                                 {yylval.symbol = hashInsert(LIT_CHAR, yytext); return LIT_CHAR;}
\"(\\[n"]|[^"\n])*[^\\]\"           {yylval.symbol = hashInsert(LIT_STRING, yytext); return LIT_STRING;}
([0-9]+\.[0-9]+|\.[0-9]+|[0-9]+\.)  {yylval.symbol = hashInsert(LIT_REAL, yytext); return LIT_REAL;}
[0-9]+                              {yylval.symbol = hashInsert(LIT_INT, yytext); return LIT_INT;}
.                                   { return TOKEN_ERROR; }

<COMMENT>"*/"   {BEGIN(INITIAL);}
<COMMENT>"\n"   { ++lineNumber; }
<COMMENT>.

%%

void initMe(void){
	hashInit();
}

int getLineNumber(void){
	return lineNumber;
}

int isRunning(void){
	return running;
}

int yywrap(){
    running = 0;
    return 1;
}
