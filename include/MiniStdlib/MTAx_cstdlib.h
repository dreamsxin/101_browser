#ifndef _MTAx_cstdlib_h
#define _MTAx_cstdlib_h

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __GNUC__
#define _byteswap_ulong __builtin_bswap32
#define _byteswap_uint64 __builtin_bswap64
#endif

#ifdef __cplusplus
}
#endif

#endif
