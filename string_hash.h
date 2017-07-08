#include "generic.h"

#define STR_HASH_MAX_ELEMENTS		256
typedef struct _STR_HASH_ENTRY
    {
    char *key;
    void *value;
    int shouldFreeValue;
    struct _STR_HASH_ENTRY *next;
    } STR_HASH_ENTRY;

typedef struct _STR_HASH_TABLE
    {
    STR_HASH_ENTRY *hashEntry[STR_HASH_MAX_ELEMENTS];
    } STR_HASH_TABLE;

unsigned int StrHashCalcHash(const char *input);

STR_HASH_ENTRY* StrHashSearch(
    STR_HASH_TABLE *hashTable,
    const char *key);

int StrHashUpdate(
    STR_HASH_TABLE *hashTable,
    const char *key,
    void *value,
    int shouldFreeValue);

STR_HASH_ENTRY* StrHashCreateNewEntry(
    const char *key,
    void *value,
    int shouldFreeValue);

void StrHashFree(STR_HASH_TABLE *hashTable);

