#ifndef _BinarySearch_h
#define _BinarySearch_h

#include "Algorithm/CompareResult.h"
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Does a binary search to find *in_toFind and returns 
 * a value x: 0<=x<=in_count-1: the index or (size_t) -1 if nothing
 * was found.
 * 
 * Parameters:
 * 
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
size_t binarySearch(void* in_pFirst, size_t in_count, size_t in_size, const void* in_toFind
					);

#ifdef __cplusplus
}
#endif

#endif
