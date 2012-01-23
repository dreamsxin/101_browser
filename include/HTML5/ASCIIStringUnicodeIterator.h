/*
 * Copyright 2008-2011 Wolfgang Keller
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

#ifndef _ASCIIStringUnicodeIterator_h
#define _ASCIIStringUnicodeIterator_h

#include "MiniStdlib/declspec.h"
#include "Util/Iterator.h"
#include "Unicode/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	// INV:ASCIIStringUnicodeIterator_h_13: characterPointer != NULL
	char* characterPointer;
	UnicodeCodePoint codePoint;
} ASCIIStringUnicodeIteratorState;

DLLEXPORT SingleIterator asciiStringUnicodeIterator_create();

/*
 * PRE:ASCIIStringUnicodeIterator_h_23: string != NULL
 * 
 * POST:ASCIIStringUnicodeCyclicIterator_h_28: the result obeys INV:ASCIIStringUnicodeIterator_h_13
 */
DLLEXPORT ASCIIStringUnicodeIteratorState asciiStringUnicodeIteratorState_create(char* string);

#ifdef __cplusplus
}
#endif

#endif
