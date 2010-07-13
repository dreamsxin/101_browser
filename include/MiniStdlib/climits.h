#ifndef _OZO_climits_h

#ifdef __cplusplus
#include <climits>
#else
#include <limits.h>
#endif

/*
 * g++ does not define this - so we define it
 */
#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)(-1)) 
#endif

#endif
