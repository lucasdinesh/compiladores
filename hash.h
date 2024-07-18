#ifndef HASH
#define HASH

#define HASH_SIZE 997

#define SYMBOL_LIT_INTEGER 1
#define SYMBOL_LIT_REAL 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_LIT_CHAR 4
#define SYMBOL_LIT_TRUE 5
#define SYMBOL_LIT_FALSE 6
#define SYMBOL_IDENTIFIER 7
#define SYMBOL_VARIABLE 8
#define SYMBOL_FUNCTION 9
#define SYMBOL_VECTOR 10
#define SYMBOL_PARAMETER 11


#define DATATYPE_CHAR 1
#define DATATYPE_BOOL 2
#define DATATYPE_FLOAT 3
#define DATATYPE_INT 4

typedef struct hash_node{
    int type;
    int datatype;
    char* text;
    struct hash_node* next;
} hash_node;

void hashInit(void);
hash_node* hashInsert(int type, char* text);
hash_node* hashFind(char *text);
int hashAddress(char *text);
void hashPrint(void);
int hash_check_undeclared(void);

#endif