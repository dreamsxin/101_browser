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

#include <assert.h>
#include <stdlib.h> // for NULL
#include "HTML5/ASCIIStringUnicodeIterator.h"

void* asciiStringUnicodeIterator_get(void* in_pIteratorState)
{
	ASCIIStringUnicodeIteratorState* pIteratorState = (ASCIIStringUnicodeIteratorState*) in_pIteratorState;

	assert(*pIteratorState->characterPointer >= 0);
	assert(*pIteratorState->characterPointer <= 127);

	pIteratorState->codePoint = *pIteratorState->characterPointer;

	if (pIteratorState->codePoint == 0)
		return NULL;
	else
		return &pIteratorState->codePoint;
}

IterateResult asciiStringUnicodeIterator_next(void* in_pIteratorState)
{
	ASCIIStringUnicodeIteratorState* pIteratorState = (ASCIIStringUnicodeIteratorState*) in_pIteratorState;

	assert(*pIteratorState->characterPointer >= 0);
	assert(*pIteratorState->characterPointer <= 127);

	if (*pIteratorState->characterPointer)
	{
		pIteratorState->characterPointer++;

		assert(*pIteratorState->characterPointer >= 0);
		assert(*pIteratorState->characterPointer <= 127);

		if (*pIteratorState->characterPointer)
		{
			return IterateResultOK;
		}
		else
		{
			return IterateResultToInvalidState;
		}
	}
	else
	{
		return IterateResultToInvalidState;
	}
}

SingleIterator asciiStringUnicodeIterator_create()
{
	SingleIterator out_si = { &asciiStringUnicodeIterator_get, &asciiStringUnicodeIterator_next };
	return out_si;
}

ASCIIStringUnicodeIteratorState asciiStringUnicodeIteratorState_create(char* string)
{
	ASCIIStringUnicodeIteratorState out_state;
	// INV:ASCIIStringUnicodeIterator_c_59; follows from PRE:ASCIIStringUnicodeIterator_h_23
	assert(string != NULL);
	// ASGN:ASCIIStringUnicodeIterator_c_63
	out_state.characterPointer = string;
	out_state.codePoint = 0; /* Setting it deliberately to an invalid value */
	/*
	 * From INV:ASCIIStringUnicodeIterator_c_59 and
	 * ASGN:ASCIIStringUnicodeIterator_c_63
	 * we get
	 * POST:ASCIIStringUnicodeCyclicIterator_h_28
	 */
	return out_state;
}
