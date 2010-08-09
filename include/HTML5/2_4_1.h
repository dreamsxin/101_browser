#ifndef _2_4_1_h
#define _2_4_1_h

/* 2.4.1 Common parser idioms
 * Draft Standard — 6th of February 2010
 */

#include "Util/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

bool isSpaceCharacter(UnicodeCodePoint in_p);

/* not implemented yet */
// bool isWhiteSpaceCharacter(UnicodeCodePoint in_p)

bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p);

// not implemented yet
// bool collectSequenceOfCharacters()

#ifdef __cplusplus
}
#endif

#endif
