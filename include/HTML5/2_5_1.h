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

#ifndef _2_5_1_h
#define _2_5_1_h

/* 
 * 2.5.1 Common parser idioms
 * Draft Standard - 6th of February 2010
 */

// for FILE*
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include "Util/Iterator.h"
#include "Unicode/Unicode.h"
#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT bool isSpaceCharacter(UnicodeCodePoint in_p);

DLLEXPORT bool isWhite_SpaceCharacter(UnicodeCodePoint in_p);

DLLEXPORT bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p);

// not implemented yet
// bool collectSequenceOfCharacters()

/*!
 * Return value: same as iterateWhile
 */
DLLEXPORT bool skipWhitespace(SingleIterator in_iterator, void* in_iteratorState);

/*!
 * Return value: same as iterateWhile
 */
DLLEXPORT bool skipWhite_SpaceCharacters(SingleIterator in_iterator, void* in_iteratorState);

#ifdef __cplusplus
}
#endif

#endif
