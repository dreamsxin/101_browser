#ifndef _MTAx_MiniStdlib_climits_h
#define _MTAx_MiniStdlib_climits_h

#ifdef __cplusplus
#include <climits>
#else
#include <limits.h>
#endif

/*
 * gcc/g++ does not define this - so we define it
 */
#ifndef SIZE_MAX
#define SIZE_MAX ((size_t)(-1)) 
#endif

#endif
