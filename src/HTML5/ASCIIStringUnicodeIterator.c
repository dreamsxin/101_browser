#include <assert.h>
// for NULL
#include <stdlib.h>
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
	out_state.characterPointer = string;
	out_state.codePoint = 0; /* Setting it deliberately to an invalid value */
	return out_state;
}
