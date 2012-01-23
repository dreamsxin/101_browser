/*
 * Copyright 2008-2012 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _2_3_h
#define _2_3_h

#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"
#include "Util/Iterator.h"
#include "Unicode/Unicode.h"

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
DLLEXPORT bool compareStringsCaseSensitive(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1);

DLLEXPORT bool compareStringsASCIICaseInsensitive(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1);

/* Not implemented yet */
#if 0
bool compareStringsCompatibilityCaseless(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1);
#endif

UnicodeCodePoint tokenToASCIILowercase(UnicodeCodePoint in_p);
UnicodeCodePoint tokenToASCIIUppercase(UnicodeCodePoint in_p);

DLLEXPORT void convertStringToASCIILowercase(char* in_string);
DLLEXPORT void convertStringToASCIIUppercase(char* in_string);

DLLEXPORT bool prefixMatch(SingleIterator in_it, void* in_patternState, void* in_stringState);

#ifdef __cplusplus
}
#endif

#endif
