#include "generic.h"

/* Implement the BST (Binary search tree) */
typedef struct _BST
    {
    char *key;
    void *value;
    int shouldFreeValue;	/* Should we call the free the memory allocated for the value */
    struct _BST *lesser;
    struct _BST *greater;
    } BST;

/* Prototypes for public BST functions */
BST* BstSearch(
    BST *bst,
    const char *value);

BST* BstInsert(
    BST **bst,
    const char *key,
    void *value,
    int shouldFreeValue);

BST *BstCreateNode(
    const char *key,
    void *value,
    int shouldFreeValue);

void BstFree(BST *bst);

void BstPrint(
    BST *bst,
    char ascending);

