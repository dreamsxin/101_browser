#ifndef _PropList_h
#define _PropList_h

/* For FILE */
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
/*!
 * out_ppIntervals has to be a pointer to a pointer to Interval<UnicodeCodePoint>;
 * the intervals will be allocated
 */
bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals, size_t* out_intervalsCount);

/*!
 * in_ppIntervals has to be a pointer to a pointer to Interval<UnicodeCodePoint>;
 * the intervals will be freed
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
void freeIntervalsFromPropList(void* in_ppIntervals);

// Internal functions

/*!
 * The function converts the hex token token (i. e. token obeys 
 * (token >= '0' && token <= '9') || (token >= 'A' && token <= 'F')
 * ) into the corresponding number.
 * 
 * If token does not obey this condition 0xFF ((uint8_t) -1) is returned.
 */
uint8_t readHexDigit(char token);

#ifdef __cplusplus
}
#endif

#endif
