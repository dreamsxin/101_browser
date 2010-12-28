#ifndef _OZO_cstdint_h

# ifdef __cplusplus
/*
 * Visual Studio 2010 is the first version to 
 * contain cstdint. In the versions before we
 * have to use boost. Also gcc does not like this
 */
#  if defined(_MSC_VER) && (_MSC_VER < 1600)
#include "contrib/pstdint.h"
#  elif defined(__GNUC__)
#include <stdint.h>
#  else
#include <cstdint>
#  endif
# else
#  if defined(_MSC_VER) && (_MSC_VER < 1600)
#include "contrib/pstdint.h"
#  else
#include <stdint.h>
#  endif
# endif

#endif
