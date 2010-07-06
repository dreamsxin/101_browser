#ifndef _OZO_cstdint_h

#ifdef __cplusplus
/*
 * Visual Studio 2010 is the first version to 
 * contains cstdint. In the versions before we
 * have to use boost. Also gcc does not like this
 */
#if defined(_MSC_VER) && (_MSC_VER > 1500)
#include <cstdint>
#else
#include "boost/cstdint.hpp"
using namespace boost; // This is a hack - you should not use this in header files
#endif
#else
#if defined(_MSC_VER)
#if  (_MSC_VER > 1500)
#include <stdint.h>
#else
#include "contrib/pstdint.h"
#endif
#endif
#endif


#endif
