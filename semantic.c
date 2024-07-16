#include "semantic.h"

int SemanticErrors = 0;

void check_and_set_declarations(AST *node)
{
    int i = 0;
    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_DECVAR:
        if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR: variable '%s' already declared\n", node->symbol->text);
                ++SemanticErrors;
            }
            node->symbol->type = SYMBOL_VARIABLE;
        }
        else
            fprintf(stderr, "node has symbol null");

        break;
    case AST_DECFUNC:
            if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR: variable '%s' already declared\n", node->symbol->text);
                ++SemanticErrors;
            }
            node->symbol->type = SYMBOL_FUNCTION;
        }
        else
            fprintf(stderr, "node has symbol null");
        break;
    case AST_DECVET:
                if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR: variable '%s' already declared\n", node->symbol->text);
                ++SemanticErrors;
            }
            node->symbol->type = SYMBOL_VECTOR;
        }
        else
            fprintf(stderr, "node has symbol null");
        break;

    default:
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
        check_and_set_declarations(node->son[i]);
}

void check_undeclared()
{
    SemanticErrors+= hash_check_undeclared();
}