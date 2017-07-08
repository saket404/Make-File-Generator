/**************************************************
* str_list.c
* Created by: Prathiphan Dubey 57070503454 (Lucky)
*             Saket Khandelwal 57070503483
**************************************************
*/
#include "str_list.h"


/*******************************************************************************
Create a new string list entry (node)
Input:
	value: new node's value
Return:
	pointer to the newly created node
	NULL if an error occurred
*******************************************************************************/
STR_LIST * StrListCreateNewNode(const char *value)
    {
    STR_LIST *node;

    if (value == NULL || value[0] == 0)
        return NULL;

    node = (STR_LIST*)calloc(1, sizeof(STR_LIST));
    if (node == NULL)
        return NULL;

    node->value = (char*)calloc(sizeof(char), strlen(value) + 1);
    if (node->value == NULL)
        {
        free(node);
        return NULL;
        }
    strcpy(node->value, value);

    return node;
    }

/*******************************************************************************
Insert new node to the head (beginning) of the string list
Input:
	strList: the string list to perform operation
	newValue: value of the node to add
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int StrListInsertHead(
    STR_LIST **strList,
    const char *newValue)
    {
    STR_LIST *node;

    node = StrListCreateNewNode(newValue);
    if (node == NULL)
        return 0;

    if (*strList)
        node->next = *strList;

    *strList = node;
    return 1;
    }

/*******************************************************************************
Insert new node to the end (tail) of the string list
Input:
	strList: the string list to perform operation
	newValue: value of the node to add
Return:
1 if successful
0 otherwise
*******************************************************************************/
int StrListInsertTail(
    STR_LIST **strList,
    const char *newValue)
    {
    STR_LIST *node;
    STR_LIST *prevNode = NULL;
    node = *strList;

    /* Move to the end of string list */
    while (node)
        {
        prevNode = node;
        node = node->next;
        }

    node = StrListCreateNewNode(newValue);
    if (node == NULL)
        return 0;

    if (prevNode)
        prevNode->next = node;
    else
        *strList = node;
    return 1;
    }

/*******************************************************************************
Free the memory allocated for the given string list
Input:
	strList: the string list to perform operation
Return:
	void
*******************************************************************************/
void StrListFree(STR_LIST* strList)
    {
    STR_LIST *node;
    STR_LIST *nextNode;
    node = strList;
    while (node)
        {
        nextNode = node->next;
        if (node->value)
            free(node->value);
        free(node);
        node = nextNode;
        }
    }

