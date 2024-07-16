#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER


#include "hash.h"
#include "ast.h"

extern int SemanticErrors;

void check_and_set_declarations(AST *node);
void check_undeclared();
int get_semantic_errors();


#endif