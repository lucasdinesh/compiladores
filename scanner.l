

%%

[-;{}+:] { fprintf(stderr, "KW_ESPECIAL %c\n", yytext[0]);}
[ \n\t]+
"<=" { fprintf(stderr, "OPERATOR_LE\n");}
">=" { fprintf(stderr, "OPERATOR_GE\n");}
"==" { fprintf(stderr, "OPERATOR_EQ\n");}
"!=" { fprintf(stderr, "OPERATOR_DIF\n");}
.       { fprintf(stderr, "%s", yytext); }

%%

int main()
{
yylex();
}

int yywrap()
{
return 1;
}