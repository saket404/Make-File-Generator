#include "generic.h"

typedef struct STR_LIST
    {
    char *value;
    struct STR_LIST *next;
    } STR_LIST;

STR_LIST *StrListCreateNewNode(const char *value);

int StrListInsertHead(
    STR_LIST **strList,
    const char *newValue);

int StrListInsertTail(
    STR_LIST **strList,
    const char *newValue);

void StrListFree(STR_LIST* strList);
