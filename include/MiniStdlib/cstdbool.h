#ifndef _cstdbool_h
#define _cstdbool_h

#if defined(_MSC_VER)
# ifndef __cplusplus

#  include "MiniStdlib/cstdint.h"

typedef uint8_t bool;
#define true 1
#define false 0

# endif
#else
# ifdef __cplusplus
#  include <cstdbool>
# else
#  include <stdbool.h>
# endif
#endif

#endif
