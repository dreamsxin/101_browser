#ifndef _cstdbool_h
#define _cstdbool_h

#if defined(_MSC_VER)
# ifndef __cplusplus

#  include "MiniStdlib/cstdint.h"

typedef uint8_t _Bool;

#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

# endif
#else
# ifndef __cplusplus
#  include <stdbool.h>
# else
#  if sizeof(bool) != 1
#   error sizeof(bool) != 1
#  endif
# endif
#endif

#endif
