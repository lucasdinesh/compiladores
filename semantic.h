#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdbool.h>
#include "hash.h"
#include "ast.h"

extern int semanticErrors;

//DATATYPÈ CHECK
int isDatatypeCompatible(int datatype1, int datatype2);
int isLogicalOperation(int nodetype);
int isRelationalOperation(int nodetype);
int isArithmeticOperation(int nodetype);
int isNumerical(int datatype);
int isChar(int datatype);
int isBoolean(int datatype);
int isInteger(int datatype);
int validToArithmetic(int dataType);
int isNumber(AST *node);


void checkAndSetDeclarations(AST *node);
void checkUndeclared();
int getDatatype();
void checkOperands(AST *node);
void setNodeTypes(AST *node);
int checkSemantic(AST *root);





#endif