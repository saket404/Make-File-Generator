/**************************************************
* string_hash.c
* Created by: Prathiphan Dubey 57070503454 (Lucky)
*             Saket Khandelwal 57070503483
**************************************************
*/
#include "string_hash.h"

/*******************************************************************************
Calculate the hash value for the given key
Input:
	key: the key to calculate hash value
Return:
	the hash value of the given key
*******************************************************************************/
unsigned int StrHashCalcHash(const char *key)
    {
    unsigned long sum = 0;
    unsigned long mult;
    size_t i, j;
    size_t n, m;

    if (key == NULL) return 0;
    n = strlen(key);
    if (n == 0) return 0;
    m = n / 4;

    for (i = 0; i < m; i++)
        {
        mult = 1;
        for (j = 0; j < 4; j++)
            {
            sum += (mult * key[i * 4 + j]);
            mult *= 256;
            }
        }

    j = m * 4;
    m = n % 4;
    mult = 1;
    for (i = 0; i < m; i++)
        {
        sum += (mult * key[i + j]);
        mult *= 256;
        }

    return (sum % STR_HASH_MAX_ELEMENTS);
    }

/*******************************************************************************
Search the hash table for the given key
Input:
	hashTable: the hash table to perform operation
	key: key to search for
Return:
	pointer to the entry that matches the given key
	NULL otherwise
*******************************************************************************/
STR_HASH_ENTRY* StrHashSearch(STR_HASH_TABLE *hashTable, const char *key)
    {
    int hashCode;
    STR_HASH_ENTRY *entry;
    if (hashTable == NULL)
        return NULL;

    hashCode = StrHashCalcHash(key);
    entry = hashTable->hashEntry[hashCode];
    while (entry)
        {
        if (strcmp(entry->key, key) == 0)
            return entry;
        entry = entry->next;
        }
    return NULL;
    }

/*******************************************************************************
Update an item in the hash table, if item doesn't exist then create a new one
Input:
	hashTable: the hash table to perform operation
	key: the new item's key
	data: the new item's value
	shouldFreeValue: whether we need to free memory allocated for value
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int StrHashUpdate(
    STR_HASH_TABLE *hashTable,
    const char *key,
    void *value,
    int shouldFreeValue)
    {
    int hashCode;
    STR_HASH_ENTRY *entry;
    STR_HASH_ENTRY *prevEntry = NULL;
    if (hashTable == NULL || key == NULL || key[0] == 0)
        return 0;

    hashCode = StrHashCalcHash(key);
    entry = hashTable->hashEntry[hashCode];
    while (entry)
        {
        if (strcmp(entry->key, key) == 0)
            {
            /* Update entry */
            if (entry->shouldFreeValue && entry->value)
                free(entry->value);
            entry->value = value;
            entry->shouldFreeValue = shouldFreeValue;
            return 1;
            }
        prevEntry = entry;
        entry = entry->next;
        }

    entry = StrHashCreateNewEntry(key, value, shouldFreeValue);
    if (entry == NULL)
        return 0;

    if (prevEntry == NULL)
        /* Empty list */
        hashTable->hashEntry[hashCode] = entry;
    else
        prevEntry->next = entry;

    return 1;
    }

/*******************************************************************************
Create a new hash entry
Input:
	key: the new item's key
	data: the new item's value
	shouldFreeValue: whether we need to free memory allocated for value
Return:
	pointer to the newly created entry
	NULL if an error occurred
*******************************************************************************/
STR_HASH_ENTRY* StrHashCreateNewEntry(
    const char *key,
    void *value,
    int shouldFreeValue)
    {
    STR_HASH_ENTRY *entry;
    if (key == NULL || key[0] == 0)
        return NULL;

    /* Allocate memory for the new entry */
    entry = (STR_HASH_ENTRY*)calloc(1, sizeof(STR_HASH_ENTRY));
    if (entry == NULL)
        return NULL;

    /* Allocate memory and copy key content */
    entry->key = (char*)calloc(sizeof(char), strlen(key) + 1);
    if (entry->key == NULL)
        {
        free(entry);
        return NULL;
        }
    strcpy(entry->key, key);
    entry->value = value;
    entry->shouldFreeValue = shouldFreeValue;
    return entry;
    }

/*******************************************************************************
Free memory allocated for the given hash table
Input:
	hashTable: the hash table to perform operation
Return:
	void
*******************************************************************************/
void StrHashFree(STR_HASH_TABLE *hashTable)
    {
    int i;
    STR_HASH_ENTRY *entry;
    STR_HASH_ENTRY *nextEntry;

    if (hashTable == NULL)
        return;
    for (i = 0; i < STR_HASH_MAX_ELEMENTS; i++)
        {
        entry = hashTable->hashEntry[i];
        while (entry)
            {
            nextEntry = entry->next;
            if (entry->shouldFreeValue && entry->value)
                free(entry->value);
            free(entry->key);
            free(entry);
            entry = nextEntry;
            }
        }
    }
