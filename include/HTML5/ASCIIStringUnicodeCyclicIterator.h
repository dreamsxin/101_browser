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

#ifndef _MTAx_HTML5_ASCIIStringUnicodeCyclicIterator_h
#define _MTAx_HTML5_ASCIIStringUnicodeCyclicIterator_h

/*!
 * This type of iterator was created, because of a potential bug found in
 * universalCompare (2_3.h/2_3.c) that ignored the IterateResult.
 * While this does not lead to problems currently (since we don't iterate in
 * a cyclic way) this is a missuse of the Iterator interfaced.
 * 
 * To discourage this bad practise Wolfgang Keller wrote this kind of 
 * iterator that causes an infinite loop in those cases.
 */

#include "MiniStdlib/declspec.h"
#include "Util/Iterator.h"
#include "Util/Unicode.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	// INV:ASCIIStringUnicodeCyclicIterator_h_23: beginOfString != NULL
	// INV:ASCIIStringUnicodeCyclicIterator_h_24: *beginOfString != 0
	char* beginOfString;
	// INV:ASCIIStringUnicodeCyclicIterator_h_26: characterPointer != NULL
	// INV:ASCIIStringUnicodeCyclicIterator_h_27: *characterPointer != 0
	char* characterPointer;
	UnicodeCodePoint codePoint;
} ASCIIStringUnicodeCyclicIteratorState;

DLLEXPORT SingleIterator asciiStringUnicodeCyclicIterator_create();

/*
 * PRE:ASCIIStringUnicodeCyclicIterator_h_38: string != NULL
 * PRE:ASCIIStringUnicodeCyclicIterator_h_39: *string != 0
 * 
 * POST:ASCIIStringUnicodeCyclicIterator_h_41: the result obeys INV:ASCIIStringUnicodeCyclicIterator_h_23
 * POST:ASCIIStringUnicodeCyclicIterator_h_42: the result obeys INV:ASCIIStringUnicodeCyclicIterator_h_24
 * POST:ASCIIStringUnicodeCyclicIterator_h_43: the result obeys INV:ASCIIStringUnicodeCyclicIterator_h_26
 * POST:ASCIIStringUnicodeCyclicIterator_h_44: the result obeys INV:ASCIIStringUnicodeCyclicIterator_h_27
 */
DLLEXPORT ASCIIStringUnicodeCyclicIteratorState asciiStringUnicodeCyclicIteratorState_create(char* string);

#ifdef __cplusplus
}
#endif

#endif
