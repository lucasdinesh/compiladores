// AST - Abstract Syntax Tree
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#ifndef AST_HEADER
#define AST_HEADER

#define MAX_SONS 4


#define AST_SYMBOL 1
#define AST_ADD 2
#define AST_SUB 3
#define AST_DIV 4
#define AST_MULT 5
#define AST_GREATHER 7
#define AST_LESS 8
#define AST_AND 9
#define AST_OR 10
#define AST_ATTR 11
#define AST_DECVAR 12
#define AST_DECVET 13
#define AST_DECFUNC 14
#define AST_LCMD 15
#define AST_NOT 16
#define AST_LE 17
#define AST_GE 18
#define AST_EQ 19
#define AST_DIF 20
#define AST_TYPECHAR 21
#define AST_TYPEINT 22
#define AST_TYPEFLOAT 23
#define AST_TYPEBOOL 24
#define AST_VECINIT 25
#define AST_VECREST 26
#define AST_VECATTR 27
#define AST_IF 28
#define AST_IFELSE 29
#define AST_WHILE 30
#define AST_PRINT 31
#define AST_READ 32
#define AST_RETURN 33
#define AST_VEC_CALL 34
#define AST_ARGL 36
#define AST_ARGLREST 37
#define AST_FUNC_CALL 38
#define AST_PRINTWDECL 39
#define AST_CMDBLOCK 40
#define AST_LCMDINIT 41
#define AST_SEMICOLON 42
#define AST_DECL 43
#define AST_PARAML 44
#define AST_PARAMLREST 45
#define AST_PARAM 46
#define AST_PAREN 47




typedef struct astnode 
{
    int type;
    int datatype;
    hash_node *symbol;
    struct astnode*son[MAX_SONS];
} AST;

AST *astCreate(int type, hash_node *symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astPrint(AST *node, int level);
void uncompileAST(AST *root, FILE *file);

#endif

// END OF FILE