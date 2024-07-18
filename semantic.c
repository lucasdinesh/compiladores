#include "semantic.h"

int SemanticErrors = 0;

int getDatatype(AST *son)
{
    if (son)
    {
        switch (son->type)
        {
        case AST_TYPEINT:
            return DATATYPE_INT;
        case AST_TYPEBOOL:
            return DATATYPE_BOOL;
        case AST_TYPEFLOAT:
            return DATATYPE_FLOAT;
        case AST_TYPECHAR:
            return DATATYPE_CHAR;
        default:
            break;
        }
    }
}

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
            node->symbol->datatype = getDatatype(node->son[0]);
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
            node->symbol->datatype = getDatatype(node->son[0]);
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
            node->symbol->datatype = getDatatype(node->son[0]);
        }
        else
            fprintf(stderr, "node has symbol null");
        break;
    case AST_PARAM:
        if (node->symbol)
        {
            if (node->symbol->type != SYMBOL_IDENTIFIER)
            {
                fprintf(stderr, "SEMANTIC ERROR: variable '%s' already declared\n", node->symbol->text);
                ++SemanticErrors;
            }
            node->symbol->type = SYMBOL_PARAMETER;
            node->symbol->datatype = getDatatype(node->son[0]);
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
    SemanticErrors += hash_check_undeclared();
}

int is_number(AST *son)
{
    // VALID AS NUMBERS or CHAR (ARITHMETIC)
    if (
        (son->type == AST_ADD || son->type == AST_SUB || son->type == AST_DIV || son->type == AST_MULT || (son->type == AST_SYMBOL && (son->symbol->type == SYMBOL_LIT_INTEGER || son->symbol->type == SYMBOL_LIT_CHAR || (son->symbol->type == SYMBOL_VARIABLE && son->symbol->datatype == DATATYPE_INT))) || (son->type == AST_FUNC_CALL && son->symbol->datatype == DATATYPE_INT)))
        return 1;
    else
        return 0;
}

int isDatatypeCompatible(int datatype1, int datatype2)
{
    return ((isInteger(datatype1) || isChar(datatype1)) && (isInteger(datatype2) || isChar(datatype2))) || (datatype1 == datatype2);
}

int isInteger(int datatype)
{
    return (datatype == DATATYPE_INT);
}

int isBoolean(int datatype)
{
    return (datatype == DATATYPE_BOOL);
}

int isChar(int datatype)
{
    return (datatype == DATATYPE_CHAR);
}

int isNumerical(int datatype)
{
    return (datatype == DATATYPE_INT || datatype == DATATYPE_FLOAT);
}

int isArithmeticOperation(int nodetype)
{
    return (nodetype == AST_ADD || nodetype == AST_SUB || nodetype == AST_MULT || nodetype == AST_DIV);
}

int isRelationalOperation(int nodetype)
{
    return (nodetype == AST_EQ || nodetype == AST_GREATHER || nodetype == AST_LESS || nodetype == AST_LE || nodetype == AST_GE || nodetype == AST_DIF);
}

int isLogicalOperation(int nodetype)
{
    return (nodetype == AST_AND || nodetype == AST_OR || nodetype == AST_NOT);
}

void setNodeTypes(AST *node)
{
    if (node == NULL)
        return;

    for (int i = 0; i < MAX_SONS; i++)
    {

        setNodeTypes(node->son[i]);
    }

    if (node->type == AST_SYMBOL)
    {
        if (node->symbol->type == SYMBOL_VECTOR || node->symbol->type == SYMBOL_FUNCTION)
        {

            fprintf(stderr, "SEMANTIC ERROR: function/vector used as scalar variable.\n");
            SemanticErrors++;
        }
        node->datatype = node->symbol->datatype;
    }
    else if (node->type == AST_FUNC_CALL || node->type == AST_VEC_CALL)
    {
        node->datatype = node->symbol->datatype;
    }
    else if (node->type == AST_PAREN)
    {
        node->datatype = node->son[0]->datatype;
    }
    else if (isArithmeticOperation(node->type))
    {

        AST *son0 = node->son[0];
        AST *son1 = node->son[1];
        if (!isDatatypeCompatible(son0->datatype, son1->datatype) || son0->datatype == DATATYPE_BOOL || son1->datatype == DATATYPE_BOOL)
        {

            fprintf(stderr, "SEMANTIC ERROR: Arithmetic operation with incompatible data types.\n");
            SemanticErrors++;
        }
        node->datatype = son0->datatype > son1->datatype ? son0->datatype : son1->datatype; // IF IS AN ARTHMETIC OPERATION WITH CHAR AND INT, USE ALWAYS INT
    }
    else if (isRelationalOperation(node->type))
    {

        AST *son0 = node->son[0];
        AST *son1 = node->son[1];
        if (!isDatatypeCompatible(son0->datatype, son1->datatype) || son0->datatype == DATATYPE_BOOL || son1->datatype == DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR: Relational operation with incompatible data types.\n");
            SemanticErrors++;
        }
        node->datatype = DATATYPE_BOOL;
    }
    else if (isLogicalOperation(node->type))
    {
        if (node->type == AST_NOT)
        {
            if (node->son[0]->datatype != DATATYPE_BOOL)
            {
                fprintf(stderr, "SEMANTIC ERROR: Logical operation with incompatible data types.\n");
                SemanticErrors++;
            }
        }
        else if (node->son[0]->datatype != DATATYPE_BOOL || node->son[1]->datatype != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR: Logical operation with incompatible data types.\n");
            SemanticErrors++;
        }
        node->datatype = DATATYPE_BOOL;
    }
    else if (node->datatype == AST_PRINTWDECL)
    {
        node->datatype = getDatatype(node->son[0]);
    }
}

void check_operands(AST *node)
{

    int i = 0;
    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_ADD:
        if (!(is_number(node->son[0])))
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for ADD \n");
        break;
        if (!(is_number(node->son[1])))
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for ADD \n");
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
        check_operands(node->son[i]);
}
