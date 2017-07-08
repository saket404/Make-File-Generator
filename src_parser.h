#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum TokenTypes
    {
    TokenUnknown,
    TokenKeyword,
    TokenIdentifier,
    TokenIntegerConstant,
    TokenFloatingPointConstant,
    TokenLiteralString,
    TokenCharSequence,
    TokenOperator,
    TokenPunctuation,
    TokenAssignmentOperator,
    };

enum ParserState
    {
    StateNormal,
    StatePreprocessor,
    StateLiteralString,
    StateCharSequence,
    StateEscapeChar,
    StateEscapeString,
    StateStartComment,
    StateLineComment,
    StateBlockComment,
    StateBlockCommentEnding,

    StatePlus,		/* Parsing "+", "++" */
    StateMinus,		/* Parsing "-", "--" */
    StateOr,		/* Parsing "|", "||" */
    StateAnd,		/* Parsing "&", "&&" */
    StateEqual,		/* Parsing "=", "==" */
    StateNot,		/* Parsing "!", "!=" */
    StateLess,		/* Parsing "<", "<=", "<<" */
    StateGreat,		/* Parsing ">", ">=", ">>" */

    };

#define MAX_TOKEN_LEN		256

typedef struct _TOKEN_ENTRY
    {
    char token[MAX_TOKEN_LEN + 1];
    unsigned long line;
    enum TokenTypes tokenType;
    struct _TOKEN_ENTRY *next;
    } TOKEN_ENTRY;


typedef struct _C_PARSER_CONTEXT
    {
    char token[MAX_TOKEN_LEN + 1];
    size_t tokenPos;
    enum ParserState state;
    unsigned long line;
    } PARSER_CONTEXT;


void SrcParser_InitContext(PARSER_CONTEXT *context);

int SrcParser_Tokenize(
    char ch,
    PARSER_CONTEXT *context,
    TOKEN_ENTRY** tokenList);

void SrcParser_FreeTokenList(TOKEN_ENTRY *tokenList);


