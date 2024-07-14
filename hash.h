#ifndef HASH
#define HASH

#define HASH_SIZE 997

#define SYMBOL_LIT_INTEGER 1
#define SYMBOL_LIT_FLOAT 2
#define SYMBOL_LIT_STRING 3
#define SYMBOL_LIT_CHAR 4
#define SYMBOL_LIT_TRUE 5
#define SYMBOL_LIT_FALSE 6
#define SYMBOL_IDENTIFIER 7

typedef struct hash_node{
    int type;
    char* text;
    struct hash_node* next;
} hash_node;

void hashInit(void);
hash_node* hashInsert(int type, char* text);
hash_node* hashFind(char *text);
int hashAddress(char *text);
void hashPrint(void);

#endif