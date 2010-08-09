#ifndef _2_3_h
#define _2_3_h

#include "MiniStdlib/cstdbool.h"
#include "Util/Iterator.h"
#include "Util/Unicode.h"

/*
 * 2.3 Case-sensitivity and string comparison
 * Draft Standard - 6th of August 2010
 */

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Returns true if the tokens are identical; false otherwise
 */
bool compareTokensCaseSensitive(UnicodeCodePoint p1, UnicodeCodePoint p2);
bool compareTokensASCIICaseInsensitive(UnicodeCodePoint p1, UnicodeCodePoint p2);

/*
 * Returns true if the strings are identical; false otherwise
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
bool compareStringsCaseSensitive(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool compareStringsASCIICaseInsensitive(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1);

/* Not implemented yet */
#if 0
bool compareStringsCompatibilityCaseless(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1);
#endif

UnicodeCodePoint tokenToASCIILowercase(UnicodeCodePoint in_p);
UnicodeCodePoint tokenToASCIIUppercase(UnicodeCodePoint in_p);

#ifdef _WIN32
__declspec(dllexport)
#endif
void convertStringToASCIILowercase(char* in_string);
#ifdef _WIN32
__declspec(dllexport)
#endif
void convertStringToASCIIUppercase(char* in_string);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool prefixMatch(SingleIterator in_it, void* in_patternState, void* in_stringState);

#ifdef __cplusplus
}
#endif

#endif
