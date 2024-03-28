%{
#include "tokens.h"
int running = 1;
%}

%%

[-,;{}+:\[\]\(\)\*/\%<>&|~=] {return yytext[0];}
[ \n\t]+
"bool" { return KW_BOOL;}
"int"   { return KW_INT;}
"char"  { return KW_CHAR;}
"float" { return KW_FLOAT;}
"if"    { return KW_IF;}
"else"  { return KW_ELSE;}
"while" { return KW_WHILE;}
"read"  { return KW_READ;}
"return" { return KW_RETURN;}
[a-zA-Z_][a-zA-Z0-9_]* { return TK_IDENTIFIER;}
"<=" { return OPERATOR_LE;}
">=" { return OPERATOR_GE;}
"==" { return OPERATOR_EQ;}
"!=" { return OPERATOR_DIF;}
.       { return TOKEN_ERROR; }

%%

int main()
{
int tok;
while(1)
{

tok = yylex();
if(running == 0) break;

switch(tok) {
    case KW_IF: fprintf(stderr, "KW_IF\n"); break;
    case KW_ELSE: fprintf(stderr, "KW_ELSE\n"); break;
    case KW_BOOL: fprintf(stderr, "KW_BOOL\n"); break;
    case KW_WHILE: fprintf(stderr, "KW_WHILE\n"); break;
    case KW_READ: fprintf(stderr, "KW_READ\n"); break;
    case KW_CHAR: fprintf(stderr, "KW_CHAR\n"); break;
    case KW_FLOAT: fprintf(stderr, "KW_FLOAT\n"); break;
    case KW_RETURN: fprintf(stderr, "KW_RETURN\n"); break;
    case KW_INT: fprintf(stderr, "KW_INT\n"); break;
    case OPERATOR_LE: fprintf(stderr, "OPERATOR_LE\n"); break;
    case OPERATOR_GE: fprintf(stderr, "OPERATOR_GE\n"); break;
    case OPERATOR_EQ: fprintf(stderr, "OPERATOR_EQ\n"); break;
    case OPERATOR_DIF: fprintf(stderr, "OPERATOR_DIF\n"); break;
    case TOKEN_ERROR: fprintf(stderr, "TOKEN_ERROR\n"); break;
    case 37 ... 38:
    case 40 ... 45:
    case 47:
    case 58 ... 62:
    case 91:
    case 93:
    case 123 ... 126:
        fprintf(stderr, "%d\n", tok); break;
    default: fprintf(stderr, "UNEXPECTED TOKEN %d\n", tok); break;
    }
}

}

int yywrap()
{
running = 0;
return 1;
}