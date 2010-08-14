#ifndef _2_4_1_h
#define _2_4_1_h

/* 2.4.1 Common parser idioms
 * Draft Standard - 6th of February 2010
 */

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
bool allocWhitespaceIntervals(FILE* in_propListFile, void** in_pWhitespaceIntervals, 
							  size_t* in_pWhitespaceIntervalsCount);
#ifdef _WIN32
__declspec(dllexport)
#endif
void freeWhitespaceIntervals(void** in_pWhitespaceIntervals);

#ifdef _WIN32
__declspec(dllexport)
#endif
extern int isWhiteSpaceCharacter(UnicodeCodePoint in_p, void* in_pWhitespaceIntervals, 
						   size_t in_whitespaceIntervalsCount);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p);

// not implemented yet
// bool collectSequenceOfCharacters()

#ifdef __cplusplus
}
#endif

#endif
