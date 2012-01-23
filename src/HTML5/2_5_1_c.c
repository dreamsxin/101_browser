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

#include <assert.h>
#include "HTML5/2_5_1.h"
#include "Algorithm/IterateWhile.h"

bool skipWhitespaceKernel(const void* in_pUnicodeCodePoint, const void* in_pUserdata)
{
	UnicodeCodePoint *pUnicodeCodePoint = (UnicodeCodePoint*) in_pUnicodeCodePoint;

	assert(pUnicodeCodePoint != NULL);

	if (pUnicodeCodePoint != NULL)
		return isSpaceCharacter(*pUnicodeCodePoint);
	else
		return false;
}

bool skipWhitespace(SingleIterator in_iterator, void* in_iteratorState)
{
	return iterateWhile(in_iterator, in_iteratorState, NULL, &skipWhitespaceKernel);
}

bool skipWhite_SpaceCharactersKernel(const void* in_pUnicodeCodePoint, const void* in_pUserdata)
{
	UnicodeCodePoint *pUnicodeCodePoint = (UnicodeCodePoint*) in_pUnicodeCodePoint;

	assert(pUnicodeCodePoint != NULL);

	if (pUnicodeCodePoint != NULL)
		return isWhite_SpaceCharacter(*pUnicodeCodePoint);
	else
		return false;
}

bool skipWhite_SpaceCharacters(SingleIterator in_iterator, void* in_iteratorState)
{
	return iterateWhile(in_iterator, in_iteratorState, NULL, &skipWhite_SpaceCharactersKernel);
}
