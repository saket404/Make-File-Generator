/**************************************************
* function_scanner.c
* Created by: Prathiphan Dubey 57070503454 (Lucky)
*             Saket Khandelwal 57070503483
**************************************************
*/
#include "function_scanner.h"
#include "src_parser.h"

/*******************************************************************************
Add a function and the source file that references to it to the internal data
Input:
	srcFile: source file path name
	fnName: function name that source file references to
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int FnScanner_AddRefFunction(
    const char *srcFile,
    const char *fnName,
    GENERATOR_DATA *genData)
    {
    BST *srcFileNode;
    STR_HASH_TABLE *fnHashTable;

    if (srcFile == NULL || fnName == NULL || genData == NULL)
        return 0;

    /* Locate the node of the file path name */
    srcFileNode = BstSearch(genData->srcTree, srcFile);
    if (srcFileNode)
        {
        fnHashTable = (STR_HASH_TABLE*)srcFileNode->value;
        if (fnHashTable == NULL)
            return 0;
        }
    else
        {
        /* If it doesn't exist, create a new one */
        fnHashTable = (STR_HASH_TABLE*)calloc(1, sizeof(STR_HASH_TABLE));
        if (fnHashTable == NULL)
            return 0;
        srcFileNode = BstInsert(&(genData->srcTree), srcFile, fnHashTable, 0);
        if (srcFileNode == NULL)
            {
            free(fnHashTable);
            return 0;
            }
        }

    /* Map the function name and the source file name */
    return StrHashUpdate(fnHashTable, fnName, NULL, 0);
    }

/*******************************************************************************
Add a function and the source file that implements it to the internal data
Input:
	srcFile: source file path name
	fnName: function name that source file implements
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int FnScanner_AddImplFunction(
    const char *srcFile,
    const char *fnName,
    GENERATOR_DATA *genData)
    {
    BST *fnNameNode;
    char *implSrcName;

    if (srcFile == NULL || srcFile[0] == 0 ||
            fnName == NULL || fnName[0] == 0 ||
            genData == NULL)
        return 0;

    /* Process the main function separately */
    if (strcmp(fnName, "main") == 0)
        return StrListInsertHead(&genData->mainSrcFiles, srcFile);

    /* Process other function */
    /* Locate the node of the function name */
    fnNameNode = BstSearch(genData->fnTree, fnName);
    if (fnNameNode)
        /* This function already has implementation */
        return 0;

    /* Node name doesn't exist, create a new one */
    implSrcName = (char*)calloc(sizeof(char), strlen(srcFile) + 1);
    if (implSrcName == NULL)
        return 0;
    strcpy(implSrcName, srcFile);
    fnNameNode = BstInsert(&(genData->fnTree), fnName, implSrcName, 1);
    if (fnNameNode == NULL)
        {
        free(implSrcName);
        return 0;
        }

    return 1;
    }

/*******************************************************************************
Add each function found in a source file to internal data storage
Input:
	srcFile: source file path name
	fnName: function name
	lnk: the link between the source file and the function
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int FnScanner_AddFoundFunction(
    const char *srcFile,
    const char *fnName,
    SrcFnLink lnk,
    GENERATOR_DATA *genData)
    {
    switch (lnk)
        {
        case LinkImpl:
            return FnScanner_AddImplFunction(srcFile, fnName, genData);
        case LinkReference:
            return FnScanner_AddRefFunction(srcFile, fnName, genData);
        }

    return 0;
    }

/*******************************************************************************
Check whether the given token is a function reference
Input:
	token: the token to check
Return:
	1 if the given token is a function reference
	0 otherwise
*******************************************************************************/
int FnScanner_IsTokenFunctionRef(TOKEN_ENTRY *token)
    {
    enum FunctionRefState state = FnRefStateBegin;
    TOKEN_ENTRY *node;

    if (token == NULL) return 0;
    node = token;
    while (node && state != FnRefStateEnd)
        {
        switch (state)
            {
            case FnRefStateBegin:
                if (node->tokenType != TokenIdentifier)
                    return 0;
                state = FnRefStateFnName;
                break;
            case FnRefStateFnName:
                if (node->tokenType == TokenPunctuation &&
                        strcmp(node->token, "(") == 0)
                    state = FnRefStateFnOpen;
                else
                    return 0;
                break;
            case FnRefStateFnOpen:
                switch (node->tokenType)
                    {
                    case TokenPunctuation:
                        if (strcmp(node->token, ")"))
                            return 0;
                        state = FnRefStateFnClose;
                        break;
                    case TokenIdentifier:
                    case TokenLiteralString:
                    case TokenCharSequence:
                    case TokenFloatingPointConstant:
                    case TokenIntegerConstant:
                        state = FnRefStateParam;
                        break;
                    default:
                        return 0;
                    }
                break;
            case FnRefStateParam:
                if (node->tokenType == TokenPunctuation &&
                        strcmp(node->token, ")") == 0)
                    state = FnRefStateFnClose;
                break;
            case FnRefStateFnClose:
                if (node->tokenType == TokenPunctuation &&
                        strcmp(node->token, "{") == 0)
                    return 0;
                state = FnRefStateEnd;
                break;
            case FnRefStateEnd:
                /* Do nothing */
                break;
            }
        node = node->next;
        }
    return (state == FnRefStateEnd);
    }

/*******************************************************************************
Check whether the given token is a function implementation
Input:
	token: the token to check
Return:
	1 if the given token is a function implementation
	0 otherwise
*******************************************************************************/
int FnScanner_IsTokenFunctionImpl(TOKEN_ENTRY *token)
    {
    enum FunctionImplState state = FnImplStateBegin;
    TOKEN_ENTRY *node;

    if (token == NULL) return 0;
    node = token;
    while (node && state != FnImplStateEnd)
        {
        switch (state)
            {
            case FnImplStateBegin:
                if (node->tokenType != TokenIdentifier)
                    return 0;
                state = FnImplStateFnName;
                break;
            case FnImplStateFnName:
                if (node->tokenType == TokenPunctuation &&
                        strcmp(node->token, "(") == 0)
                    state = FnImplStateFnOpen;
                else
                    return 0;
                break;
            case FnImplStateFnOpen:
                switch (node->tokenType)
                    {
                    case TokenPunctuation:

                        if (strcmp(node->token, ")"))
                            return 0;
                        state = FnImplStateFnClose;
                    case TokenIdentifier:
                    case TokenKeyword:
                        state = FnImplStateArg;
                        break;
                    default:
                        return 0;
                    }
                break;
            case FnImplStateArg:
                switch (node->tokenType)
                    {
                    case TokenPunctuation:
                        if (strcmp(node->token, ")") == 0)
                            state = FnImplStateFnClose;
                        else if (strcmp(node->token, ",") == 0)
                            ;
                        else
                            return 0;
                        break;
                    case TokenKeyword:
                    case TokenIdentifier:
                    case TokenOperator:
                        break;
                    default:
                        return 0;
                    }
                break;
            case FnImplStateFnClose:
                if (node->tokenType == TokenPunctuation &&
                        strcmp(node->token, "{") == 0)
                    state = FnImplStateEnd;
                else
                    return 0;
                break;
            case FnImplStateEnd:
                /* Do nothing */
                break;
            }
        node = node->next;
        }

    return (state == FnImplStateEnd);
    }

/*******************************************************************************
Scan the source file for function references and implementations
Input:
	srcName: source file name
	genData: makefile generator internal data
	tokenList: list of tokens in the given source file
Return:
	-1 if an error occurred
	otherwise the number of functions found
*******************************************************************************/
int FnScanner_ScanForFunctions(
    const char* srcName,
    GENERATOR_DATA *genData,
    TOKEN_ENTRY *tokenList)
    {
    TOKEN_ENTRY *token;
    int cnt = 0;

    if (tokenList == NULL)
        return -1;

    token = tokenList;
    while (token)
        {
        if (FnScanner_IsTokenFunctionRef(token))
            {
            FnScanner_AddFoundFunction(srcName, token->token, LinkReference, genData);
            cnt++;
            }
        else if (FnScanner_IsTokenFunctionImpl(token))
            {
            FnScanner_AddFoundFunction(srcName, token->token, LinkImpl, genData);
            cnt++;
            }
        token = token->next;
        }
    return cnt;
    }

/*******************************************************************************
Scan the source file for function references and implementations
Input:
	srcFilePath: source file path to scan
	srcName: source file name
	genData: makefile generator internal data
Return:
	-1 if an error occurred
	otherwise the number of functions found
*******************************************************************************/
int FnScanner_ScanSrcFile(
    const char *srcFile,
    const char *srcName,
    GENERATOR_DATA *genData)
    {
    FILE *inputFile;
    size_t cnt;
    size_t i;
    char buffer[1024];
    int result;

    PARSER_CONTEXT *context;
    TOKEN_ENTRY *tokenList = NULL;

    if (srcFile == NULL) return -1;

    inputFile = fopen(srcFile, "rb");
    if (inputFile == NULL)
        {
        printf("Failed to open input file for reading %s\n", srcFile);
        return -1;
        }

    context = (PARSER_CONTEXT*)calloc(1, sizeof(PARSER_CONTEXT));
    if (context == NULL)
        {
        printf("Failed to allocate buffer for parser context.\n");
        fclose(inputFile);
        return -1;
        }
    SrcParser_InitContext(context);

    int col = 0;
    while (!feof(inputFile))
        {
        cnt = fread(buffer, sizeof(char), 1024, inputFile);
        if (cnt == 0)
            {
            printf("Failed to read input file.\n");
            free(context);
            fclose(inputFile);
            return -1;
            }

        for (i = 0; i < cnt; i++)
            {
            col++;
            if (buffer[i] == '\n')
                col = 0;
            if (!SrcParser_Tokenize(buffer[i], context, &tokenList))
                {
                printf("Failed to parse input file at line: %lu, column: %d.\n", context->line, col);
                fclose(inputFile);

                free(context);
                if (tokenList)
                    SrcParser_FreeTokenList(tokenList);
                return -1;
                }
            }

        }
    fclose(inputFile);

    free(context);

    result = FnScanner_ScanForFunctions(srcName, genData, tokenList);

    if (tokenList)
        SrcParser_FreeTokenList(tokenList);


    return result;
    }
