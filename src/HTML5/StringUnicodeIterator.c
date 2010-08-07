#include <assert.h>
#include "HTML5/StringUnicodeIterator.h"

void* stringUnicodeIterator_get(void* in_pStringUnicodeIteratorState)
{
	StringUnicodeIteratorState* pStringUnicodeIteratorState = in_pStringUnicodeIteratorState;

	assert(*pStringUnicodeIteratorState->characterPointer >= 0);
	assert(*pStringUnicodeIteratorState->characterPointer <= 127);

	pStringUnicodeIteratorState->codePoint = *pStringUnicodeIteratorState->characterPointer;

	if (pStringUnicodeIteratorState->codePoint == 0)
		return NULL;
	else
		return &pStringUnicodeIteratorState->codePoint;
}

IterateResult stringUnicodeIterator_next(void* in_pStringUnicodeIteratorState)
{
	StringUnicodeIteratorState* pStringUnicodeIteratorState = in_pStringUnicodeIteratorState;

	assert(*pStringUnicodeIteratorState->characterPointer >= 0);
	assert(*pStringUnicodeIteratorState->characterPointer <= 127);

	if (*pStringUnicodeIteratorState->characterPointer)
	{
		pStringUnicodeIteratorState->characterPointer++;
		return IterateResultOK;
	}
	else
	{
		return IterateResultToInvalidState;
	}
}

SingleIterator stringUnicodeIterator_create()
{
	SingleIterator out_si = { &stringUnicodeIterator_get, &stringUnicodeIterator_next };
	return out_si;
}

StringUnicodeIteratorState stringUnicodeIteratorState_create(char* string)
{
	StringUnicodeIteratorState out_state;
	out_state.characterPointer = string;
	out_state.codePoint = 0; /* Setting it deliberately to an invalid value */
	return out_state;
}
