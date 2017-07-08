#include "generic.h"
#include "binary_search_tree.h"
#include "string_hash.h"
#include "str_list.h"

typedef enum _SrcFnLink
    {
    LinkReference,		/* Source file references to function */
    LinkImpl,			/* Source file implements the function */
    } SrcFnLink;

/* Data structure represents the internal data of the generator */
typedef struct _GENERATOR_DATA
    {
    BST *fnTree;	/* BST for function names and source files
					 that implement them */
    BST *srcTree;	/* BST for source file names and functions they reference to */
    STR_LIST *mainSrcFiles;	/* List of source files containing the main function */
    } GENERATOR_DATA;

int FnScanner_ScanSrcFile(
    const char *srcFile,
    const char *srcName,
    GENERATOR_DATA *genData);


