#ifndef _BinarySearch_h
#define _BinarySearch_h

#include "Algorithm/CompareResult.h"
#include "MiniStdlib/cstdint.h"
 // for size_t
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Does a binary search to find *in_toFind and returns 
 * a value x: 0<=x<=in_count-1: the index or (size_t) -1 if nothing
 * was found.
 * 
 * Parameters:
 * in_pToFind:  the element (it may be of another type than the array to follow) to find
 * in_pFirst:   pointer to the first element in the array
 * in_count:    number of elements in the array
 * in_size:     size of an array element
 * in_pCompare: a comparator function taking a pointer to in_pToFind and a pointer of an array
 *              element
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
size_t binarySearch(const void* in_pToFind, const void* in_pFirst, size_t in_count, size_t in_size,
					IntermediateCompareResult (*in_pCompare)(const void* in_pToFind, const void* in_pDatum));

#ifdef __cplusplus
}
#endif

#endif
