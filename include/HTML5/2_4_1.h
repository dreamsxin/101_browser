#ifndef _2_4_1_h
#define _2_4_1_h

/* 
 * 2.4.1 Common parser idioms
 * Draft Standard - 6th of February 2010
 */

// for FILE*
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include "Util/Iterator.h"
#include "Util/Unicode.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
bool isSpaceCharacter(UnicodeCodePoint in_p);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool allocWhite_SpaceCharacterIntervals(FILE* in_propListFile, void** in_pWhitespaceIntervals, 
                              size_t* in_pWhitespaceIntervalsCount);
#ifdef _WIN32
__declspec(dllexport)
#endif
void freeWhite_SpaceCharacterIntervals(void** in_pWhitespaceIntervals);

#ifdef _WIN32
__declspec(dllexport)
#endif
extern bool isWhite_SpaceCharacter(UnicodeCodePoint in_p, void* in_pWhitespaceIntervals, 
						   size_t in_whitespaceIntervalsCount);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p);

// not implemented yet
// bool collectSequenceOfCharacters()

#ifdef _WIN32
__declspec(dllexport)
#endif
void skipWhitespace(SingleIterator in_iterator, void* in_iteratorState);

#ifdef _WIN32
__declspec(dllexport)
#endif
void skipWhite_SpaceCharacters(SingleIterator in_iterator, void* in_iteratorState, 
							   void* in_pWhitespaceIntervals, size_t in_whitespaceIntervalsCount);

#ifdef __cplusplus
}
#endif

#endif
