#ifndef TAC_H
#define TAC_H

#include "ast.h"

#define TAC_SYMBOL 0
#define TAC_COPY 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_GREAT 6
#define TAC_LESS 7
#define TAC_GE 8
#define TAC_LE 9
#define TAC_EQ 10
#define TAC_DIF 11
#define TAC_AND 12
#define TAC_OR 13
#define TAC_NOT 14
#define TAC_LABEL 15
#define TAC_BEGINFUN 16
#define TAC_ARGLREST 17
#define TAC_ENDFUN 18
#define TAC_IFZ 19
#define TAC_JUMP 20
#define TAC_CALL 21
#define TAC_ARG 22
#define TAC_RETURN 23
#define TAC_PRINT 24
#define TAC_READ 25
#define TAC_VECATTR 26
#define TAC_VEC_CALL 27
#define TAC_PARAM 28

typedef struct tac_struct{
	int type;
	hash_node *res;
	hash_node *op1;
	hash_node *op2;
	struct tac_struct *prev;
	struct tac_struct *next; 
} tac;

tac* newTac(int type, hash_node *res, hash_node *op1, hash_node *op2);
tac* tacJoin(tac* l1, tac* l2);
tac* reverseTacList(tac* l);
void printAllTacs(tac* l);
void printTac(tac* l);
tac* createTacs(AST *node, hash_node *currentLoopLabel);

tac* createIf(tac* son[]);
tac* generateBinaryOpTac(int type, tac* son[]);
tac* createWhileLoop(tac* son[], hash_node *label);
tac* createFunction(tac* symbol, tac* params, tac* code);

#endif