#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

hash_node *Table[HASH_SIZE];

void hashInit(void)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Table[i] = 0;
    }
}

int getDatatypeFromHash(int type)
{
    if (type)
    {

        switch (type)
        {
        case SYMBOL_LIT_INTEGER:
            return DATATYPE_INT;
        case SYMBOL_LIT_FALSE:
        case SYMBOL_LIT_TRUE:
            return DATATYPE_BOOL;
        case SYMBOL_LIT_REAL:
            return DATATYPE_FLOAT;
        case SYMBOL_LIT_CHAR:
            return DATATYPE_CHAR;
        default:
            break;
        }
    }
    return 0;
}

hash_node *hashInsert(int type, char *text)
{

    hash_node *newnode = hashFind(text);
    if (newnode != NULL)
        return newnode;

    int address = hashAddress(text);

    newnode = (hash_node *)calloc(1, sizeof(hash_node));

    newnode->type = type;
    newnode->datatype = getDatatypeFromHash(type);
    newnode->text = calloc(strlen(text) + 1, sizeof(char));
    strcpy(newnode->text, text);
    makeAssemblyTempName(newnode);
    newnode->next = Table[address];
    Table[address] = newnode;

    return newnode;
}

hash_node *hashFind(char *text)
{
    hash_node *node;
    int address = hashAddress(text);
    for (node = Table[address]; node != NULL; node = node->next)
    {
        if (strcmp(text, node->text) == 0)
            return node;
    }
    return 0;
}

int hashAddress(char *text)
{
    int address = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        address = (address * text[i]) % HASH_SIZE + 1;
    }
    return address - 1;
}

void hashPrint(void)
{
    hash_node *node;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (Table[i] != NULL)
        {
            for (node = Table[i]; node != NULL; node = node->next)
            {
                printf("Table[%d] - type: %d, text: %s, datatype: %d\n", i, node->type, node->text, node->datatype);
            }
        }
    }
}


void printASM(FILE *fout)
{
    int tempIterator = 0;
    hash_node *node;

    fprintf(fout, "##DATA SECTION\n"
                  "\t.data\n");

    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (Table[i] != NULL)
        {
            for (node = Table[i]; node != NULL; node = node->next)
            {
                // if (node->initVariableValue != NULL)
                //     printf("Table[%d] - type: %d, text: %s, datatype: %d, node->initVariableValue: %s\n", i, node->type, node->text, node->datatype, node->initVariableValue);
                // else
                //     printf("Table[%d] - type: %d, text: %s, datatype: %d\n", i, node->type, node->text, node->datatype);

                if (node->type == SYMBOL_VARIABLE)
                {
                    switch (node->datatype)
                    {
                    case DATATYPE_INT:
                    case DATATYPE_BOOL:
                        fprintf(fout, "_%s:\t.long\t%s\n", node->text, node->initVariableValue);
                        break;
                    case DATATYPE_FLOAT:
                        fprintf(fout, "_%s:\t.float\t%s\n", node->text, node->initVariableValue);
                        break;
                    case DATATYPE_CHAR:
                        fprintf(fout, "_%s:\t.byte\t%d\n", node->text, node->initVariableValue[0]);
                        break;
                    }
                }

                if (node->type == SYMBOL_LIT_STRING)
                {
                    fprintf(fout,
                            "\t.section\t .rodata\n_%s:\t.string\t%s\n", node->tempAssemblyName, node->text);
                }
            }
        }
    }
}

int hash_check_undeclared(void)
{
    int undeclared = 0;
    hash_node *node;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        if (Table[i] != NULL)
        {
            for (node = Table[i]; node != NULL; node = node->next)
                if (node->type == SYMBOL_IDENTIFIER)
                {
                    fprintf(stderr, "SEMANTIC ERROR: identifier %s undeclared\n", node->text);
                    ++undeclared;
                }
        }
    }
    return undeclared;
}

hash_node** getHash(){
    return Table;
}


hash_node *makeTemp()
{
    static int serialNumber = 0;
    static char buffer[128];
    sprintf(buffer, "mYWeeirT_emp%d", serialNumber++);
    return hashInsert(SYMBOL_IDENTIFIER, buffer);
}

hash_node *makeLabel()
{
    static int serialNumber = 0;
    static char buffer[128];
    sprintf(buffer, "mYLabe_l%d", serialNumber++);
    return hashInsert(SYMBOL_IDENTIFIER, buffer);
}

void makeAssemblyTempName(hash_node *node)
{

    static int tempIterator = 0;
    static char buffer[128];
    sprintf(buffer, "tempStringName_l%d", tempIterator++);
    node->tempAssemblyName = calloc(strlen(buffer) + 1, sizeof(char));
    strcpy(node->tempAssemblyName, buffer);
}