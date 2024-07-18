#include "semantic.h"

int semanticErrors = 0;
AST *ROOT;

int checkSemantic(AST *root)
{
    ROOT = root;

    checkAndSetDeclarations(root);
    checkUndeclared();
    checkOperands(root);
    setNodeTypes(root);

    return semanticErrors;
}

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

void checkAndSetDeclarations(AST *node)
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
                semanticErrors++;
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
                semanticErrors++;
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
                semanticErrors++;
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
                semanticErrors++;
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
        checkAndSetDeclarations(node->son[i]);
}

void checkUndeclared()
{
    semanticErrors += hash_check_undeclared();
}

int isInteger(int datatype)
{
    return (datatype == DATATYPE_INT);
}

int isFloat(int datatype)
{
    return (datatype == DATATYPE_FLOAT);
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

int isDatatypeCompatible(int datatype1, int datatype2)
{
    return ((isInteger(datatype1) || isChar(datatype1)) && (isInteger(datatype2) || isChar(datatype2))) || (datatype1 == datatype2);
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
            semanticErrors++;
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
        if (!isDatatypeCompatible(son0->datatype, son1->datatype) || isBoolean(son0->datatype) || isBoolean(son1->datatype))
        {

            fprintf(stderr, "SEMANTIC ERROR: Arithmetic operation with incompatible data types.\n");
            semanticErrors++;
        }
        node->datatype = son0->datatype > son1->datatype ? son0->datatype : son1->datatype; // IF IS AN ARTHMETIC OPERATION WITH CHAR AND INT, USE ALWAYS INT
    }
    else if (isRelationalOperation(node->type))
    {

        AST *son0 = node->son[0];
        AST *son1 = node->son[1];
        if (!isDatatypeCompatible(son0->datatype, son1->datatype) || isBoolean(son0->datatype) || isBoolean(son1->datatype))
        {
            fprintf(stderr, "SEMANTIC ERROR: Relational operation with incompatible data types.\n");
            semanticErrors++;
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
                semanticErrors++;
            }
        }
        else if (node->son[0]->datatype != DATATYPE_BOOL || node->son[1]->datatype != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR: Logical operation with incompatible data types.\n");
            semanticErrors++;
        }
        node->datatype = DATATYPE_BOOL;
    }
    else if (node->datatype == AST_PRINTWDECL)
    {
        node->datatype = getDatatype(node->son[0]);
    }
}

int validToArithmetic(int dataType)
{
    return (isInteger(dataType) || isChar(dataType) || isFloat(dataType));
}

int isNumber(AST *son)
{
    // VALID FOR ARITHMETIC METHODS
    if (
        (son->type == AST_ADD || son->type == AST_SUB || son->type == AST_DIV || son->type == AST_MULT || (son->type == AST_SYMBOL && ((son->symbol->type == SYMBOL_LIT_INTEGER || ((son->symbol->type == SYMBOL_VARIABLE || son->symbol->type == SYMBOL_PARAMETER) && isInteger(son->symbol->datatype))) || (son->symbol->type == SYMBOL_LIT_REAL || ((son->symbol->type == SYMBOL_VARIABLE || son->symbol->type == SYMBOL_PARAMETER) && isFloat(son->symbol->datatype))) || (son->symbol->type == SYMBOL_LIT_CHAR || ((son->symbol->type == SYMBOL_VARIABLE || son->symbol->type == SYMBOL_PARAMETER) && isChar(son->symbol->datatype))))) || (son->type == AST_FUNC_CALL && validToArithmetic(son->symbol->datatype))))
        return 1;
    else
        return 0;
}

void checkOperands(AST *node)
{

    int i = 0;
    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_ADD:
        if (!(isNumber(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for ADD \n");
            semanticErrors++;
        }
        if (!(isNumber(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for ADD \n");
            semanticErrors++;
        }
        break;
    case AST_SUB:
        if (!(isNumber(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for SUB \n");
            semanticErrors++;
        }
        if (!(isNumber(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for SUB \n");
            semanticErrors++;
        }
        break;
    case AST_DIV:
        if (!(isNumber(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for DIV \n");
            semanticErrors++;
        }
        if (!(isNumber(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for DIV \n");
            semanticErrors++;
        }
        break;
    case AST_MULT:
        if (!(isNumber(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for MULT \n");
            semanticErrors++;
        }
        if (!(isNumber(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for MULT \n");
            semanticErrors++;
        }
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
        checkOperands(node->son[i]);
}
