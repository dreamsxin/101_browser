#ifndef _PropList_h
#define _PropList_h

/* For FILE */
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
/*
 * out_ppIntervals has to be a pointer to a pointer to Interval<UnicodeCodePoint>
 * it will be allocated
 */
bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals);

#ifdef __cplusplus
}
#endif

#endif
