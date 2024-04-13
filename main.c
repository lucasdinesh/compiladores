#include <stdio.h>
#include "lex.yy.h"
#include "y.tab.c"

int yylex();
extern char *yytext;
extern FILE *yyin;

int getLineNumber();
int isRunning(void);
void initMe(void);
void hashPrint();

int main(int argc, char *argv[]){
	if(argc<2){
		fprintf(stderr,"Execute: ./etapa1 nomearquivo.\n");
		exit (1);
	}
    if (0 == (yyin = fopen(argv[1], "r"))){
		fprintf(stderr,"I'm not able to open the file %s.\n",argv[1]);
		exit (2);
	}

	initMe();

    yyparse();

    hashPrint();

    exit(0);
}

