#ifndef _IterateWhile_h
#define _IterateWhile_h

#include "MiniStdlib/cstdbool.h"
#include "Util/Iterator.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
void iterateWhile(SingleIterator in_iterator, void* in_iteratorState, void* in_pUserdata,
				  bool (*in_pConditionFunction)(const void* in_pCurrentElement, const void* in_pUserdata));

#ifdef __cplusplus
}
#endif

#endif
