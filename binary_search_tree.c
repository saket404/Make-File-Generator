/**************************************************
* binary_search_tree.c
* Created by: Prathiphan Dubey 57070503454 (Lucky)
*             Saket Khandelwal 57070503483
**************************************************
*/
#include "binary_search_tree.h"

/*******************************************************************************
Search the BST for the given value
Input:
	bst: the BST to perform operation
	key: the key to search for
Return:
	the BST node if found
	NULL otherwise
*******************************************************************************/
BST* BstSearch(
    BST *bst,
    const char *key)
    {
    int cmpResult;
    BST *node;
    if (bst == NULL || key == NULL || key[0] == 0)
        return NULL;

    /* Starting from the top of the tree */
    node = bst;
    while (node)
        {
        cmpResult = strcmp(key, node->key);
        if (cmpResult < 0)
            /* Search the lesser branch */
            node = node->lesser;
        else if (cmpResult > 0)
            /* Search the greater branch */
            node = node->greater;
        else
            /* Value found */
            return node;
        }

    return NULL;
    }

/*******************************************************************************
Insert the new node to the BST
Input:
	bst: the BST to perform operation
	key: the new node's key
	value: the new node's value
	shouldFreeValue: whether we need to free memory allocated for value
Return:
	NULL if an error occurred
	otherwise return the pointer to the newly inserted node
*******************************************************************************/
BST* BstInsert(
    BST **bst,
    const char *key,
    void *value,
    int shouldFreeValue)
    {
    int cmpResult;
    BST *node;
    BST *parentNode = NULL;
    char goLesser = 0;

    if (key == NULL || key[0] == 0)
        return NULL;

    if (*bst == NULL)
        {
        /* BST is not initialized yet, then we initialize it */
        node = BstCreateNode(key, value, shouldFreeValue);
        if (node == NULL)
            return NULL;
        *bst = node;
        return node;
        }

    node = *bst;
    while (node)
        {
        parentNode = node;
        cmpResult = strcmp(key, node->key);
        if (cmpResult < 0)
            {
            /* Search the lesser branch */
            node = node->lesser;
            goLesser = 1;
            }
        else if (cmpResult > 0)
            {
            /* Search the greater branch */
            node = node->greater;
            goLesser = 0;
            }
        else
            /* key already exists */
            return node;
        }

    if (parentNode == NULL)
        return NULL;	/* Error */

    node = BstCreateNode(key, value, shouldFreeValue);
    if (node == NULL)
        return NULL;

    if (goLesser)
        parentNode->lesser = node;
    else
        parentNode->greater = node;

    return node;
    }

/*******************************************************************************
Create a new BST node
Input:
	key: the new node's key
	value: the new node's value
	shouldFreeValue: whether we need to free memory allocated for value
Return:
	Pointer to the newly created node
	NULL if an error occurred
*******************************************************************************/
BST *BstCreateNode(
    const char *key,
    void *value,
    int shouldFreeValue)
    {
    BST *node;

    if (key == NULL)
        return NULL;
    node = (BST*)calloc(1, sizeof(BST));
    if (node == NULL)
        return NULL;
    node->key = (char*)calloc(sizeof(char), strlen(key) + 1);
    if (node->key == NULL)
        {
        free(node);
        return NULL;
        }
    strcpy(node->key, key);
    node->value = value;
    node->shouldFreeValue = shouldFreeValue;

    return node;
    }

/*******************************************************************************
Free memory allocated for the given BST
Input:
	bst: the BST to perform operation
Return:
	void
*******************************************************************************/
void BstFree(BST *bst)
    {
    if (bst)
        {
        BstFree(bst->greater);
        BstFree(bst->lesser);
        if (bst->shouldFreeValue && bst->value)
            free(bst->value);
        if (bst->key)
            free(bst->key);
        free(bst);
        }
    }

/*******************************************************************************
Print the content of all nodes of the given BST to stdout
Input:
	bst: the BST to perform operation
	ascending: 1 to display nodes in ascending order, 0 for descending order
Return:
	void
*******************************************************************************/
void BstPrint(
    BST *bst,
    char ascending)
    {
    if (bst)
        {
        if (ascending)
            {
            BstPrint(bst->lesser, ascending);
            printf("%s\n", bst->key);
            BstPrint(bst->greater, ascending);
            }
        else
            {
            BstPrint(bst->greater, ascending);
            printf("%s\n", bst->key);
            BstPrint(bst->lesser, ascending);
            }

        }
    }
