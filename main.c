#include <stdio.h>
#include <stdlib.h>
#include "tacs.h"
#include "semantic.h"

int yylex();
extern FILE *yyin;

extern int isRunning();
extern int getLineNumber();
extern void initMe();
extern void hashPrint();
extern int yyparse(); 
extern AST *getAST();
extern int checkSemanticErrors();
extern tac *getTACS();

int main(int argc, char *argv[]){
	FILE *output;  

	if (argc < 3){
		fprintf(stderr, "Call: ./etapa3 input.txt output.txt\n");
		exit(1);
    }

    if ((yyin = fopen(argv[1], "r")) == 0){
        fprintf(stderr, "Fail to open file %s\n", argv[1]);
		exit(1);
    }

    if ((output = fopen(argv[2], "w+")) == 0){
        fprintf(stderr, "Fail to open file %s\n", argv[2]);
		exit(1);
    }

	initMe();

    yyparse();

	fprintf(stderr, "Compiled Successfully.\n");

	fprintf(stderr, "Uncompiling!\n");

	uncompileAST(getAST(), output);

	fprintf(stderr, "Uncompiled Successfully!\n");

	fclose(output);

	checkSemanticErrors();

	printAllTacs(getTACS());

	generateASM(getTACS());

    exit(0);
}

