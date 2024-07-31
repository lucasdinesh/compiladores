#include "semantic.h"
#include <string.h>

int semanticErrors = 0;
AST *ROOT;

int checkSemantic(AST *root)
{
    ROOT = root;
    checkAndSetDeclarations(root);
    checkUndeclared();
    checkOperands(root);
    assignAndValidateNodeTypes(root);
    checkUsage(root);
    checkReturns(root);

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
    return 0;
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
            int lenghtVector = atoi(node->son[1]->symbol->text);

            checkVector(node->son[2], node->symbol->datatype, lenghtVector);
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

void checkVector(AST *node, int vecDataType, int vecLenght)
{
    if (vecLenght < 0)
    {
        fprintf(stderr, "SEMANTIC ERROR: Exceeded numbers of items on vector\n");
        semanticErrors++;
    }
    if (node == NULL)
        return;

    else if (node->son[0])
    {
        if (!isDatatypeCompatible(node->son[0]->symbol->datatype, vecDataType))
        {
            fprintf(stderr, "SEMANTIC ERROR:  value has dataType incompatible with Vector DataType\n");
            semanticErrors++;
        }
        else
        {
            vecLenght--;
            checkVector(node->son[1], vecDataType, vecLenght);
        }
    }
}

void checkUndeclared()
{
    semanticErrors += hash_check_undeclared();
}

int isInteger(int datatype)
{
    return (datatype == DATATYPE_INT || isChar(datatype));
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

void assignAndValidateNodeTypes(AST *node) {
    if (node == NULL) {
        return;
    }

    // Recursively set types for child nodes
    for (int i = 0; i < MAX_SONS; i++) {
        assignAndValidateNodeTypes(node->son[i]);
    }

    // Handle different node types
    switch (node->type) {
        case AST_SYMBOL:
            if (node->symbol == NULL) {
                fprintf(stderr, "SEMANTIC ERROR: NULL symbol in AST_SYMBOL node.\n");
                semanticErrors++;
                return;
            }
            
            if (node->symbol->type == SYMBOL_VECTOR || node->symbol->type == SYMBOL_FUNCTION) {
                fprintf(stderr, "SEMANTIC ERROR: Function/vector used as scalar variable.\n");
                semanticErrors++;
            }
            node->datatype = node->symbol->datatype;
            break;

        case AST_FUNC_CALL:
        case AST_VEC_CALL:
            if (node->symbol == NULL) {
                fprintf(stderr, "SEMANTIC ERROR: NULL symbol in AST_FUNC_CALL or AST_VEC_CALL node.\n");
                semanticErrors++;
                return;
            }
            node->datatype = node->symbol->datatype;
            break;

        case AST_PAREN:
            if (node->son[0] == NULL) {
                fprintf(stderr, "SEMANTIC ERROR: NULL child node in AST_PAREN node.\n");
                semanticErrors++;
                return;
            }
            node->datatype = node->son[0]->datatype;
            break;

        default:
            if (isArithmeticOperation(node->type)) {
                if (node->son[0] == NULL || node->son[1] == NULL) {
                    fprintf(stderr, "SEMANTIC ERROR: NULL child nodes in arithmetic operation.\n");
                    semanticErrors++;
                    return;
                }

                AST *son0 = node->son[0];
                AST *son1 = node->son[1];

                if (!isDatatypeCompatible(son0->datatype, son1->datatype) ||
                    isBoolean(son0->datatype) || isBoolean(son1->datatype)) {
                    fprintf(stderr, "SEMANTIC ERROR: Arithmetic operation with incompatible data types.\n");
                    semanticErrors++;
                }

                node->datatype = (son0->datatype > son1->datatype) ? son0->datatype : son1->datatype;
            }
            else if (isRelationalOperation(node->type)) {
                if (node->son[0] == NULL || node->son[1] == NULL) {
                    fprintf(stderr, "SEMANTIC ERROR: NULL child nodes in relational operation.\n");
                    semanticErrors++;
                    return;
                }

                AST *son0 = node->son[0];
                AST *son1 = node->son[1];

                if (!isDatatypeCompatible(son0->datatype, son1->datatype) ||
                    isBoolean(son0->datatype) || isBoolean(son1->datatype)) {
                    fprintf(stderr, "SEMANTIC ERROR: Relational operation with incompatible data types.\n");
                    semanticErrors++;
                }

                node->datatype = DATATYPE_BOOL;
            }
            else if (isLogicalOperation(node->type)) {
                if (node->son[0] == NULL) {
                    fprintf(stderr, "SEMANTIC ERROR: NULL child node in logical operation.\n");
                    semanticErrors++;
                    return;
                }

                if (node->type == AST_NOT) {
                    if (node->son[0]->datatype != DATATYPE_BOOL) {
                        fprintf(stderr, "SEMANTIC ERROR: Logical NOT operation with non-boolean type.\n");
                        semanticErrors++;
                    }
                } else {
                    if (node->son[0]->datatype != DATATYPE_BOOL || node->son[1]->datatype != DATATYPE_BOOL) {
                        fprintf(stderr, "SEMANTIC ERROR: Logical operation with non-boolean types.\n");
                        semanticErrors++;
                    }
                }
                node->datatype = DATATYPE_BOOL;
            }
            else if (node->type == AST_PRINTWDECL) {
                if (node->son[0] == NULL) {
                    fprintf(stderr, "SEMANTIC ERROR: NULL child node in AST_PRINTWDECL.\n");
                    semanticErrors++;
                } else {
                    node->datatype = getDatatype(node->son[0]);
                }
            }
            break;
    }
}

int validToArithmetic(int dataType)
{
    return (isInteger(dataType) || isChar(dataType) || isFloat(dataType));
}

bool isNumericOrArithmetic(AST *node) {
    if (node == NULL) {
        return false;
    }

    switch (node->type) {
        case AST_ADD:
        case AST_SUB:
        case AST_DIV:
        case AST_MULT:
            return true;

        case AST_SYMBOL: {
            if (node->symbol == NULL) {
                return false;
            }
            
            int symbolType = node->symbol->type;
            int symbolDataType = node->symbol->datatype;
            
            if (symbolType == SYMBOL_LIT_INTEGER ||
                (symbolType == SYMBOL_VARIABLE || symbolType == SYMBOL_PARAMETER) && isInteger(symbolDataType)) {
                return true;
            }

            if (symbolType == SYMBOL_LIT_REAL ||
                (symbolType == SYMBOL_VARIABLE || symbolType == SYMBOL_PARAMETER) && isFloat(symbolDataType)) {
                return true;
            }

            if (symbolType == SYMBOL_LIT_CHAR ||
                (symbolType == SYMBOL_VARIABLE || symbolType == SYMBOL_PARAMETER) && isChar(symbolDataType)) {
                return true;
            }

            return false;
        }

        case AST_FUNC_CALL:
        case AST_VEC_CALL:
            return validToArithmetic(node->symbol ? node->symbol->datatype : 0);

        default:
            return false;
    }
}

void checkOperands(AST *node)
{

    int i = 0;
    if (node == 0)
        return;

    switch (node->type)
    {
    case AST_ADD:
        if (!(isNumericOrArithmetic(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for ADD \n");
            semanticErrors++;
        }
        if (!(isNumericOrArithmetic(node->son[1])))
        {
            astPrint(node->son[1], 0);

            fprintf(stderr, "node->son[1]->symbol->datatype: %d, node->son[1]->type:%d, node->son[1]->symbol->type: %d \n",node->son[1]->datatype
            , node->son[1]->type, node->son[1]->symbol->type);
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for ADD \n");
            semanticErrors++;
        }
        break;
    case AST_SUB:
        if (!(isNumericOrArithmetic(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for SUB \n");
            semanticErrors++;
        }
        if (!(isNumericOrArithmetic(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for SUB \n");
            semanticErrors++;
        }
        break;
    case AST_DIV:
        if (!(isNumericOrArithmetic(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for DIV \n");
            semanticErrors++;
        }
        if (!(isNumericOrArithmetic(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for DIV \n");
            semanticErrors++;
        }
        break;
    case AST_MULT:
        if (!(isNumericOrArithmetic(node->son[0])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation left operand for MULT \n");
            semanticErrors++;
        }
        if (!(isNumericOrArithmetic(node->son[1])))
        {
            fprintf(stderr, "SEMANTIC ERROR: operation right operand for MULT \n");
            semanticErrors++;
        }
        break;
    }

    for (i = 0; i < MAX_SONS; i++)
        checkOperands(node->son[i]);
}

void checkUsage(AST *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case AST_ATTR:
        if (node->symbol->type != SYMBOL_VARIABLE)
        {
            fprintf(stderr, "SEMANTIC ERROR: Attribution must be to a scalar variable.\n");
            semanticErrors++;
        }
        if (!isDatatypeCompatible(node->symbol->datatype, node->son[0]->datatype))
        {
            fprintf(stderr, "SEMANTIC ERROR: Attribution with incompatible data type.\n");
            semanticErrors++;
        }
        break;
    case AST_VECATTR:
        if (node->symbol->type != SYMBOL_VECTOR)
        {
            fprintf(stderr, "SEMANTIC ERROR: Indexing only allowed for vectors.\n");
            semanticErrors++;
        }
        if (!isDatatypeCompatible(node->symbol->datatype, node->son[1]->datatype))
        {
            fprintf(stderr, "SEMANTIC ERROR: Attribution with incompatible data type.\n");
            semanticErrors++;
        }
        if (!isInteger(node->son[0]->datatype))
        {
            fprintf(stderr, "SEMANTIC ERROR: Index must be an integer.\n");
            semanticErrors++;
        }
        break;
    case AST_FUNC_CALL:
        validateFunction(node);
        break;
    case AST_VEC_CALL:
        if (node->symbol->type != SYMBOL_VECTOR)
        {
            fprintf(stderr, "SEMANTIC ERROR: only vector should be accessed with index\n");
            semanticErrors++;
        }
            break;
    case AST_READ:
        if (node->symbol->type != SYMBOL_VARIABLE)
        {
            fprintf(stderr, "SEMANTIC ERROR: read only allowed to scalar variables.\n");
            semanticErrors++;
        }
        break;
    case AST_PRINT:
        checkPrint(node->son[0]);
        break;
    case AST_IF:
    case AST_IFELSE:
    case AST_WHILE:
        if (node->son[0]->datatype != DATATYPE_BOOL)
        {
            fprintf(stderr, "SEMANTIC ERROR: Condition must be a boolean expression.\n");
            semanticErrors++;
        }
        break;
    default:
        break;
    }

    for (int i = 0; i < MAX_SONS; i++)
        checkUsage(node->son[i]);
}

void checkPrint(AST *node)
{
    if (node == NULL)
        return;
    if (node->type != AST_PRINTWDECL)
        return;
    
    if (!isDatatypeCompatible(getDatatype(node->son[0]), node->son[1]->datatype))
    {
        fprintf(stderr, "SEMANTIC ERROR: invalid print type argument.\n");
        semanticErrors++;
    }
}

void validateFunction(AST *node)
{
    AST *dec = searchForFunctionDeclaration(node->symbol->text, ROOT);
    if (dec == NULL)
    {
        fprintf(stderr, "SEMANTIC ERROR: Just functions can be called.\n");
        semanticErrors++;
    }
    else if (checkNumberOfArguments(node, dec))
    {
        checkCalledArguments(node->son[0], dec->son[1]);
    }
}

AST *searchForFunctionDeclaration(char *name, AST *node)
{

    if (node->symbol != NULL && node->type == AST_DECFUNC && strcmp(node->symbol->text, name) == 0)
    {
        return node;
    }

    for (int i = 0; i < MAX_SONS; i++)
    {
        if (node->son[i] == NULL)
            return NULL;

        AST *searching = searchForFunctionDeclaration(name, node->son[i]);
        if (searching != NULL)
            return searching;
    }
    return NULL;
}

int getAmountOfArguments(AST *node)
{
    if (node == NULL)
        return 0;
    if (node->son[1] != NULL)
        return 1 + getAmountOfArguments(node->son[1]);
    else
        return 0;
}

bool checkNumberOfArguments(AST *node, AST *dec)
{
    int calledArguments = getAmountOfArguments(node->son[0]);
    int declaredArguements = getAmountOfArguments(dec->son[1]);
    if (calledArguments != declaredArguements)
    {
        fprintf(stderr, "SEMATIC ERROR: Incompatible number of arguments.\n");
        semanticErrors++;
        return false;
    }
    return true;
}

void checkCalledArguments(AST *node, AST *dec)
{
    if (node->son[0] != NULL)
    {
        if (!isDatatypeCompatible(node->son[0]->datatype, dec->son[0]->symbol->datatype))
        {
            fprintf(stderr, "SEMANTIC ERROR: Incompatible argument types\n");
            semanticErrors++;
        }
        if (node->son[0]->type == AST_SYMBOL)
        {
            if (node->son[0]->symbol->type == SYMBOL_VECTOR)
            {
                fprintf(stderr, "SEMANTIC ERROR: Cannot pass vector as argument\n");
                semanticErrors++;
            }

            else if (node->son[0]->symbol->type == SYMBOL_FUNCTION)
            {
                fprintf(stderr, "SEMANTIC ERROR: Cannot pass function as argument\n");
                semanticErrors++;
            }
        }
        if (node->son[1] != NULL)
            checkCalledArguments(node->son[1], dec->son[1]);
    }
}

void isReturnCompatible(AST *node, int datatype)
{
    if (node == NULL)
        return;
    if (node->type == AST_RETURN)
    {
        if (!isDatatypeCompatible(node->son[0]->datatype, datatype))
        {
            printf("SEMANTIC ERROR: Return statement with wrong datatype.\n");
            semanticErrors++;
        }
    }
    for (int i = 0; i < MAX_SONS; i++)
    {
        isReturnCompatible(node->son[i], datatype);
    }
}

void checkReturns(AST *node)
{
    if (node != NULL && node->type == AST_DECFUNC)
    {
        isReturnCompatible(node, node->symbol->datatype);
    }

    for (int i = 0; i < MAX_SONS; i++)
    {
        if (node->son[i] == NULL)
            break;
        checkReturns(node->son[i]);
    }
}
