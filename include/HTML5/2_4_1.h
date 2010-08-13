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

bool isSpaceCharacter(UnicodeCodePoint in_p);

bool isWhiteSpaceCharacter(UnicodeCodePoint in_p, void* in_pWhitespaceIntervals, 
						   size_t in_pWhitespaceIntervalsCount);

bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p);

// not implemented yet
// bool collectSequenceOfCharacters()

#ifdef __cplusplus
}
#endif

#endif
