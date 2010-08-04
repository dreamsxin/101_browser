#ifndef _cstdbool_h
#define _cstdbool_h

#if defined(_MSC_VER)
# ifndef __cplusplus

#  include "MiniStdlib/cstdint.h"

/* In opposite to the standard we use */
typedef uint8_t bool;
/* instead of #define bool _Bool */

#define true 1
#define false 0
#define __bool_true_false_are_defined 1

# endif
#else
# ifdef __cplusplus
#  include <cstdbool>
# else
#  include <stdbool.h>
# endif
#endif

#endif
