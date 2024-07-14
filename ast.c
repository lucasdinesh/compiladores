#include <stdio.h>
#include <stdlib.h>
 #include "ast.h"

 #define PRINT_CASE(constant) case constant: fprintf(stderr, #constant " "); break;

AST *astCreate(int type, hash_node *symbol, AST* s0, AST* s1, AST* s2, AST* s3){
    AST*newnode;
    newnode = (AST*) calloc(1, sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = s0;
    newnode->son[1] = s1;
    newnode->son[2] = s2;
    newnode->son[3] = s3;
    return newnode;
    

}

void astPrint(AST *node, int level)
{   
    int i=0;
    if(node ==0)
        return;

    for(i=0; i<level; i++)
        fprintf(stderr, "  ");

    fprintf(stderr, "ast(");
    switch (node->type)
    {
        case AST_SYMBOL: fprintf(stderr, "AST_SYMBOL "); break;
        case AST_ADD: fprintf(stderr, "AST_ADD "); break;
        case AST_SUB: fprintf(stderr, "AST_SUB "); break;
        case AST_DIV: fprintf(stderr, "AST_DIV "); break;
        case AST_MULT: fprintf(stderr, "AST_MULT "); break;
        case AST_PERCENT: fprintf(stderr, "AST_PERCENT "); break;
        case AST_GREATHER: fprintf(stderr, "AST_GREATHER "); break;
        case AST_LESS: fprintf(stderr, "AST_LESS "); break;
        case AST_AND: fprintf(stderr, "AST_AND "); break;
        case AST_OR: fprintf(stderr, "AST_OR "); break;
        case AST_ATTR: fprintf(stderr, "AST_ATTR "); break;
        case AST_DECVAR: fprintf(stderr, "AST_DECVAR "); break;
        case AST_DECVET: fprintf(stderr, "AST_DECVET "); break;
        case AST_DECFUNC: fprintf(stderr, "AST_DECFUNC "); break;
        case AST_LCMD: fprintf(stderr, "AST_LCMD "); break;
        case AST_NOT: fprintf(stderr, "AST_NOT "); break;
        case AST_LE: fprintf(stderr, "AST_LE "); break;
        case AST_GE: fprintf(stderr, "AST_GE "); break;
        case AST_EQ: fprintf(stderr, "AST_EQ "); break;
        case AST_DIF: fprintf(stderr, "AST_DIF "); break;
        case AST_TPBYTE: fprintf(stderr, "AST_TPBYTE "); break;
        case AST_TPINT: fprintf(stderr, "AST_TPINT "); break;
        case AST_TPFLOAT: fprintf(stderr, "AST_TPFLOAT "); break;
        case AST_TPBOOL: fprintf(stderr, "AST_TPBOOL "); break;
        case AST_VECINIT: fprintf(stderr, "AST_VECINIT "); break;
        case AST_VECREST: fprintf(stderr, "AST_VECREST "); break;
        case AST_VECATTR: fprintf(stderr, "AST_VECATTR "); break;
        case AST_IF: fprintf(stderr, "AST_IF "); break;
        case AST_ELSE: fprintf(stderr, "AST_ELSE "); break;
        case AST_WHILE: fprintf(stderr, "AST_WHILE "); break;
        case AST_PRINT: fprintf(stderr, "AST_PRINT "); break;
        case AST_READ: fprintf(stderr, "AST_READ "); break;
        case AST_RETURN: fprintf(stderr, "AST_RETURN "); break;
        case AST_VEC: fprintf(stderr, "AST_VEC "); break;
        case AST_ARGL: fprintf(stderr, "AST_ARGL "); break;
        case AST_ARGLREST: fprintf(stderr, "AST_ARGLREST "); break;
        case AST_FUNC: fprintf(stderr, "AST_FUNC "); break;
        case AST_PRINTWDECL: fprintf(stderr, "AST_PRINTWDECL "); break;
        case AST_CMDBLOCK: fprintf(stderr, "AST_CMDBLOCK "); break;
        case AST_LCMDINIT: fprintf(stderr, "AST_LCMDINIT "); break;
        case AST_LCMDEND: fprintf(stderr, "AST_LCMDEND "); break;
        case AST_DECL: fprintf(stderr, "AST_DECL "); break;
        case AST_PARAML: fprintf(stderr, "AST_PARAML "); break;
        case AST_PARAMLREST: fprintf(stderr, "AST_PARAMLREST "); break;
        case AST_PARAM: fprintf(stderr, "AST_PARAM "); break;

    default: fprintf(stderr, "AST_UNKNOWN "); break;
    }

    if(node->symbol !=0)
        fprintf(stderr, "%s\n", node->symbol->text);
    else
        fprintf(stderr, "0\n");

    for(i=0; i<MAX_SONS; i++)
        astPrint(node->son[i], level+1);
}

// END OF FILE