#ifndef _IterateWhile_h
#define _IterateWhile_h

#include "MiniStdlib/cstdbool.h"
#include "Util/Iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Return value:
 * true if there is no situation where we got past the end (the
 *      iterator stays valid) or the iterator state was already invalid
 * false otherwise
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
bool iterateWhile(SingleIterator in_iterator, void* in_iteratorState, void* in_pUserdata,
				  bool (*in_pConditionFunction)(const void* in_pCurrentElement, const void* in_pUserdata));

#ifdef __cplusplus
}
#endif

#endif
