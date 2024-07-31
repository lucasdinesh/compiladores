#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER

#include <stdbool.h>
#include "hash.h"
#include "ast.h"

extern int semanticErrors;

int isDatatypeCompatible(int datatype1, int datatype2);
int isLogicalOperation(int nodetype);
int isRelationalOperation(int nodetype);
int isArithmeticOperation(int nodetype);
int isNumerical(int datatype);
int isChar(int datatype);
int isBoolean(int datatype);
int isInteger(int datatype);
int validToArithmetic(int dataType);
bool isNumericOrArithmetic(AST *node);
void checkPrint(AST *node);
void validateFunction(AST *node);
AST* searchForFunctionDeclaration(char * name, AST *node);
int getAmountOfArguments(AST *node);
bool checkNumberOfArguments(AST *node, AST *dec);
void checkCalledArguments(AST *node, AST *dec);
void isReturnCompatible(AST *node, int datatype);
void checkVector(AST *node, int vecDataType, int vecLenght);

void checkAndSetDeclarations(AST *node);
void checkUndeclared();
int getDatatype();
void checkOperands(AST *node);
void assignAndValidateNodeTypes(AST *node);
int checkSemantic(AST *root);
void checkUsage(AST *node);
void checkReturns(AST * node);

#endif