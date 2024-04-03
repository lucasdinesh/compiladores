#include <stdio.h>
#include "lex.yy.h"
#include "tokens.h"

int yylex();
extern char *yytext;
extern FILE *yyin;

int getLineNumber();
int isRunning(void);
void initMe(void);
void hashPrint();

int main(int argc, char *argv[]){
    int token;

	if(argc<2){
		fprintf(stderr,"Execute: ./etapa1 nomearquivo.\n");
		return 1;
	}
    if (0 == (yyin = fopen(argv[1], "r"))){
		fprintf(stderr,"I'm not able to open the file %s.\n",argv[1]);
		return 2;
	}

	initMe();

	while(isRunning()){
		token = yylex();
		if(!isRunning()) break;

        switch(token) {
            case KW_IF: fprintf(stderr, "KW_IF\n"); break;
            case KW_ELSE: fprintf(stderr, "KW_ELSE\n"); break;
            case KW_BOOL: fprintf(stderr, "KW_BOOL\n"); break;
            case KW_WHILE: fprintf(stderr, "KW_WHILE\n"); break;
            case KW_READ: fprintf(stderr, "KW_READ\n"); break;
            case KW_CHAR: fprintf(stderr, "KW_CHAR\n"); break;
            case KW_FLOAT: fprintf(stderr, "KW_FLOAT\n"); break;
            case KW_RETURN: fprintf(stderr, "KW_RETURN\n"); break;
            case KW_INT: fprintf(stderr, "KW_INT\n"); break;
            case TK_IDENTIFIER: fprintf(stderr, "TK_IDENTIFIER\n"); break;
            case OPERATOR_LE: fprintf(stderr, "OPERATOR_LE\n"); break;
            case OPERATOR_GE: fprintf(stderr, "OPERATOR_GE\n"); break;
            case OPERATOR_EQ: fprintf(stderr, "OPERATOR_EQ\n"); break;
            case OPERATOR_DIF: fprintf(stderr, "OPERATOR_DIF\n"); break;
            case TOKEN_ERROR: fprintf(stderr, "TOKEN_ERROR\n"); break;
            case LIT_STRING: fprintf(stderr, "LIT_STRING\n"); break;
            case LIT_INT: fprintf(stderr, "LIT_INT\n"); break;
            case LIT_REAL: fprintf(stderr, "LIT_REAL\n"); break;
            case LIT_CHAR: fprintf(stderr, "LIT_CHAR\n"); break;
            case LIT_FALSE: fprintf(stderr, "LIT_FALSE\n"); break;
            case LIT_TRUE: fprintf(stderr, "LIT_TRUE\n"); break;
            default: fprintf(stderr, "%d\n", token); break;
    }
}
    hashPrint();
}

