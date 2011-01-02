#ifndef _MTAx_Coroutine_Coroutine_h
#define _MTAx_Coroutine_Coroutine_h

#include "MiniStdlib/cstdbool.h"

#if defined(_WIN32)
# include "CoroutineWin/CoroutineWin.h"
#elif defined(__unix)
# include "CoroutinePosix/CoroutinePosix.h"
#else
# error Your OS seems not to be supported
#endif

#endif
