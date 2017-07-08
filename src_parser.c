/**************************************************
* src_parser.c
* Created by: Prathiphan Dubey 57070503454 (Lucky)
*             Saket Khandelwal 57070503483
**************************************************
*/
#include "src_parser.h"

/*******************************************************************************
Check whether the given character is a punctuation
Input:
	ch: character to check
Return:
	1 if the given character is a punctuation
	0 otherwise
*******************************************************************************/
int SrcParser_IsPunctuation(char ch)
    {
    const char punctuations[] = "[](){},:;";
    return (strchr(punctuations, ch) != NULL);
    }

/*******************************************************************************
Check whether the given character is an escape character
Input:
	ch: character to check
Return:
	1 if the given character is an escape character
	0 otherwise
*******************************************************************************/
int SrcParser_IsEscape(char ch)
    {
    return (strchr("abfnrtv'\"\\?xX0", ch) != NULL);
    }

/*******************************************************************************
Check whether the given token is an identifier
Input:
	token: token to check
Return:
	1 if the given token is an identifier
	0 otherwise
*******************************************************************************/
int SrcParser_IsIdentifier(const char *token)
    {
    int len = (int)strlen(token);
    int i;

    if (len == 0)
        return 0;

    if (token[0] != '_' && !isalpha(token[0]))
        return 0;

    for (i = 1; i < len; i++)
        {
        if (!isalnum(token[i]) && token[i] != '_') return 0;
        }
    return 1;
    }

/*******************************************************************************
Check whether the given token is a binary operator
Input:
	token: token to check
Return:
	1 if the given token is a binary operator
	0 otherwise
*******************************************************************************/
int SrcParser_IsBinaryOperator(const char *token)
    {
    const char *operators[] =
        {
        "/",
        "%",
        "*",
        "+",
        "-",
        "<<",
        ">>",
        "<",
        ">",
        "<=",
        ">=",
        "==",
        "!=",
        "&",
        "|",
        "^",
        "&&",
        "||"
        };
    const int operatorCnt = sizeof(operators) / sizeof(operators[0]);
    int i;

    for (i = 0; i < operatorCnt; i++)
        {
        if (strcmp(token, operators[i]) == 0)
            return 1;
        }
    return 0;
    }

/*******************************************************************************
Check whether the given token is an unary operator
Input:
	token: token to check
Return:
	1 if the given token is an unary operator
	0 otherwise
*******************************************************************************/
int SrcParser_IsUnaryOperator(const char *token)
    {
    const char *operators[] =
        {
        "-",
        "+",
        "~",
        "!",
        "*",
        "&",
        "sizeof",
        "++",
        "--",
        };
    const int operatorCnt = sizeof(operators) / sizeof(operators[0]);
    int i;

    for (i = 0; i < operatorCnt; i++)
        {
        if (strcmp(token, operators[i]) == 0)
            return 1;
        }
    return 0;
    }

/*******************************************************************************
Check whether the given token is an assignment operator
Input:
	token: token to check
Return:
	1 if the given token is an assignment operator
	0 otherwise
*******************************************************************************/
int SrcParser_IsAssignmentOperator(const char *token)
    {
    return (strcmp(token, "=") == 0);
    }

/*******************************************************************************
Check whether the given token is an operator
Input:
	token: token to check
Return:
	1 if the given token is an operator
	0 otherwise
*******************************************************************************/
int SrcParser_IsOperator(const char *token)
    {
    return (SrcParser_IsUnaryOperator(token) ||
            SrcParser_IsBinaryOperator(token));
    }

/*******************************************************************************
Check whether the given token is a keyword
Input:
	token: token to check
Return:
	1 if the given token is a keyword
	0 otherwise
*******************************************************************************/
int SrcParser_IsKeyword(const char *token)
    {
    const char *keywords[] =
        {
        "auto",
        "double",
        "int",
        "struct",
        "break",
        "else",
        "long",
        "switch",
        "case",
        "enum",
        "register",
        "typedef",
        "char",
        "extern",
        "return",
        "union",
        "const",
        "float",
        "short",
        "unsigned",
        "continue",
        "for",
        "signed",
        "void",
        "default",
        "goto",
        "sizeof",
        "volatile",
        "do",
        "if",
        "static",
        "while",
        };
    const int keywordCnt = sizeof(keywords) / sizeof(keywords[0]);
    int i;

    for (i = 0; i < keywordCnt; i++)
        {
        if (strcmp(token, keywords[i]) == 0)
            return 1;
        }
    return 0;

    }

/*******************************************************************************
Check whether the given token is an integer constant
Input:
	token: token to check
Return:
	1 if the given token is an integer constant
	0 otherwise
*******************************************************************************/
int SrcParser_IsIntegerConstant(const char * token)
    {
    int len = (int)strlen(token);
    int i;
    const char hexDigits[] = "0123456789ABCDEFabcdef";

    if (len == 0) return 0;
    /* Skip integer suffix */
    if (len > 1)
        {
        if (token[len - 1] == 'l' || token[len - 1] == 'L')
            {
            if (len > 2 && (token[len - 2] == 'u' || token[len - 2] == 'U'))
                len -= 2;
            else
                len -= 1;
            }
        else if (token[len - 1] == 'u' || token[len - 1] == 'U')
            {
            if (len > 2 && (token[len - 2] == 'l' || token[len - 2] == 'L'))
                len -= 2;
            else
                len -= 1;
            }
        }
    if (token[0] == '0')
        {
        /* Is octal or hexadecimal */
        if (len == 1) return 1;
        if (token[1] == 'x' || token[1] == 'X')
            {
            /* Is hexadecimal */
            if (len <= 2) return 0;
            for (i = 2; i < len; i++)
                {
                if (strchr(hexDigits, token[i]) == NULL)
                    return 0;
                }
            return 1;
            }
        else
            {
            /* Is octal */
            for (i = 1; i < len; i++)
                {
                if (token[i] < '0' || token[i] > '7')
                    return 0;
                }
            return 1;
            }
        }
    else
        {
        /* Integer number */
        for (i = 0; i < len; i++)
            {
            if (!isdigit(token[i]))
                return 0;
            }
        }

    return 1;
    }

/*******************************************************************************
Check whether the given token is a floating-point constant
Input:
	token: token to check
Return:
	1 if the given token is a floating-point constant
	0 otherwise
*******************************************************************************/
int SrcParser_IsFloatingPointConstant(const char * token)
    {
    int len = (int)strlen(token);
    int i;
    int hasFloatingPoint = -1;
    int hasExponent = -1;
    int digitCount = 0;

    if (len > 1)
        {
        if (token[len - 1] == 'L' || token[len - 1] == 'l' ||
                token[len - 1] == 'F' || token[len - 1] == 'f')
            len--;
        }

    for (i = 0; i < len; i++)
        {
        if (isdigit(token[i]))
            {
            digitCount++;
            continue;
            }

        if (token[i] == '.')
            {
            if (hasFloatingPoint >= 0 ||
                    hasExponent >= 0)
                return 0;
            if (digitCount == 0)
                {
                if (i == len - 1 ||
                        !isdigit(token[i + 1]))
                    return 0;
                }
            hasFloatingPoint = i;
            }
        else if (token[i] == 'e' || token[i] == 'E')
            {
            if (i == len - 1)
                return 0;
            hasExponent = i;
            }
        else if (token[i] == '+' || token[i] == '-')
            {
            if (i == len - 1 || hasExponent < 0 || hasExponent != (i - 1))
                return 0;
            }

        }

    if (digitCount == 0)
        return 0;

    if (hasFloatingPoint < 0 && hasExponent < 0)
        return 0;


    return 1;
    }

/*******************************************************************************
Initialize the parser context
Input:
	context: the parser context to initialize
Return:
	void
*******************************************************************************/
void SrcParser_InitContext(PARSER_CONTEXT *context)
    {
    if (context == NULL) return;

    context->line = 1;
    context->tokenPos = 0;
    context->state = StateNormal;
    }

/*******************************************************************************
Add a new token to the token list
Input:
	tokenType: token type to add
	context: the current parser context
	tokenList: the token list to add new token to
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int SrcParser_AddToken(
    enum TokenTypes tokenType,
    PARSER_CONTEXT *context,
    TOKEN_ENTRY **tokenList)
    {
    TOKEN_ENTRY *newToken;
    TOKEN_ENTRY *node;
    if (context == NULL || tokenList == NULL)
        return 0;

    newToken = (TOKEN_ENTRY*)calloc(1, sizeof(TOKEN_ENTRY));
    if (newToken == NULL)
        return 0;

    strncpy(newToken->token, context->token, MAX_TOKEN_LEN);
    newToken->token[MAX_TOKEN_LEN] = 0;
    newToken->line = context->line;
    newToken->tokenType = tokenType;

    if (*tokenList == NULL)
        *tokenList = newToken;
    else
        {
        node = *tokenList;
        while (node)
            {
            if (node->next == NULL)
                {
                node->next = newToken;
                break;
                }
            node = node->next;
            }
        }
    memset(context->token, 0, sizeof(context->token));
    context->tokenPos = 0;

    return 1;
    }

/*******************************************************************************
Classify the current token in the parser context and try adding new token
Input:
	context: the current parser context
	tokenList: the token list to add new token to
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int SrcParser_ClassifyAndAddToken(
    PARSER_CONTEXT *context,
    TOKEN_ENTRY** tokenList)
    {
    if (strlen(context->token) == 0)
        return 1;	/* Skip empty token */
    if (SrcParser_IsKeyword(context->token))
        return SrcParser_AddToken(TokenKeyword, context, tokenList);

    else if (SrcParser_IsIdentifier(context->token))
        return SrcParser_AddToken(TokenIdentifier, context, tokenList);

    else if (SrcParser_IsIntegerConstant(context->token))
        return SrcParser_AddToken(TokenIntegerConstant, context, tokenList);

    else if (SrcParser_IsFloatingPointConstant(context->token))
        return SrcParser_AddToken(TokenFloatingPointConstant, context, tokenList);

    else if (SrcParser_IsAssignmentOperator(context->token))
        return SrcParser_AddToken(TokenAssignmentOperator, context, tokenList);

    else if (SrcParser_IsOperator(context->token))
        return SrcParser_AddToken(TokenOperator, context, tokenList);
    return 0;
    }

/*******************************************************************************
Tokenize the input stream character by character
Input:
	ch: a new character to tokenize
	context: the current parser context
	tokenList: the token list to add new token to
Return:
	1 if successful
	0 otherwise
*******************************************************************************/
int SrcParser_Tokenize(
    char ch,
    PARSER_CONTEXT *context,
    TOKEN_ENTRY** tokenList)
    {
    if (context == NULL || tokenList == NULL)
        return 0;

    if (context->tokenPos == MAX_TOKEN_LEN &&
            (context->state != StateLiteralString && context->state != StateCharSequence &&
             context->state != StateEscapeString && context->state != StateEscapeChar))
        {
        /* Token too long */
        return 0;
        }
    if (ch == '\n')
        context->line++;

    /* Comment states */
    switch (context->state)
        {
        case StateLineComment:
            if (ch == '\n')
                context->state = StateNormal;
            return 1;
        case StateBlockComment:
            if (ch == '*')
                context->state = StateBlockCommentEnding;
            return 1;
        case StateBlockCommentEnding:
            if (ch == '/')
                context->state = StateNormal;
            else
                context->state = StateBlockComment;
            return 1;
        case StateEscapeChar:
            if (SrcParser_IsEscape(ch))
                {
                context->state = StateCharSequence;
                return 1;
                }
            else
                return 0;
        case StateCharSequence:
            if (ch == '\'')
                {
                context->state = StateNormal;
                return SrcParser_AddToken(TokenCharSequence, context, tokenList);
                }
            else if (ch == '\\')
                {
                context->state = StateEscapeChar;
                return 1;
                }
            else if (ch == '\n')
                return 0;
            else
                {
                if (context->tokenPos < MAX_TOKEN_LEN)
                    context->token[context->tokenPos++] = ch;
                return 1;
                }
        case StateEscapeString:
            if (SrcParser_IsEscape(ch))
                {
                context->state = StateLiteralString;
                return 1;
                }
            else
                return 0;
        case StateLiteralString:
            if (ch == '\"')
                {
                context->state = StateNormal;
                return SrcParser_AddToken(TokenLiteralString, context, tokenList);
                }
            else if (ch == '\\')
                {
                context->state = StateEscapeString;
                return 1;
                }
            else if (ch == '\n')
                return 0;
            else
                {
                if (context->tokenPos < MAX_TOKEN_LEN)
                    context->token[context->tokenPos++] = ch;
                return 1;
                }
        case StatePreprocessor:
            if (ch == '\n')
                context->state = StateNormal;
            return 1;
        case StateStartComment:
            if (ch == '/')
                {
                context->state = StateLineComment;
                return 1;
                }
            else if (ch == '*')
                {
                context->state = StateBlockComment;
                return 1;
                }
            else
                {
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
        case StatePlus:
            if (ch == '+')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateMinus:
            if (ch == '-')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateOr:
            if (ch == '|')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateAnd:
            if (ch == '&')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateEqual:
            if (ch == '=')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateNot:
            if (ch == '=')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateLess:
            if (ch == '=' || ch == '<')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateGreat:
            if (ch == '=' || ch == '>')
                {
                context->token[context->tokenPos++] = ch;
                return SrcParser_ClassifyAndAddToken(context, tokenList);
                }
            else
                {
                if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                    return 1;
                context->state = StateNormal;
                }
        case StateNormal:
            /* spaces */
            switch (ch)
                {
                case '\t':
                case ' ':
                case '\n':
                case '\r':
                    return SrcParser_ClassifyAndAddToken(context, tokenList);
                case '/':
                    context->state = StateStartComment;
                    return 1;
                case '#':
                    context->state = StatePreprocessor;
                    return 1;
                case '\"':
                    context->state = StateLiteralString;
                    return 1;
                case '\'':
                    context->state = StateCharSequence;
                    return 1;
                case '+':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StatePlus;
                    return 1;
                case '-':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateMinus;
                    return 1;
                case '|':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateOr;
                    return 1;
                case '&':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateAnd;
                    return 1;
                case '=':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateEqual;
                    return 1;
                case '!':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateNot;
                    return 1;
                case '<':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateLess;
                    return 1;
                case '>':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    context->state = StateGreat;
                    return 1;
                case '*':
                case '%':
                case '^':
                    if (!SrcParser_ClassifyAndAddToken(context, tokenList))
                        return 0;
                    context->token[context->tokenPos++] = ch;
                    return SrcParser_ClassifyAndAddToken(context, tokenList);
                default:
                    if (SrcParser_IsPunctuation(ch))
                        {
                        if (!SrcParser_ClassifyAndAddToken(context, tokenList)) return 0;
                        context->token[0] = ch;
                        if (!SrcParser_AddToken(TokenPunctuation, context, tokenList)) return 0;
                        return 1;
                        }
                    else
                        {
                        context->token[context->tokenPos++] = ch;
                        return 1;
                        }
                    break;
                }
        }
    return 0;
    }

/*******************************************************************************
Free memory allocated for the given token list
Input:
	tokenList: the token list to free memory
Return:
	void
*******************************************************************************/
void SrcParser_FreeTokenList(TOKEN_ENTRY *tokenList)
    {
    TOKEN_ENTRY *entry = tokenList;
    TOKEN_ENTRY *nextEntry;
    while (entry)
        {
        nextEntry = entry->next;
        free(entry);
        entry = nextEntry;
        }
    }
