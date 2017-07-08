#ifndef GENERIC_H
#define GENERIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#if defined(_WIN32)
#define PATH_SEPARATOR_CHAR		'\\'
#define PATH_SEPARATOR_STRING	"\\"
#elif defined(linux) || defined(__linux)
#define PATH_SEPARATOR_CHAR		'/'
#define PATH_SEPARATOR_STRING	"/"
#endif

enum FunctionRefState
    {
    FnRefStateBegin,
    FnRefStateFnName,
    FnRefStateFnOpen,
    FnRefStateParam,
    FnRefStateFnClose,
    FnRefStateEnd,
    };

enum FunctionImplState
    {
    FnImplStateBegin,
    FnImplStateFnName,
    FnImplStateFnOpen,
    FnImplStateArg,
    FnImplStateFnClose,
    FnImplStateEnd,
    };

#endif
