#include <assert.h>
// for NULL
#include <stdlib.h>
#include "HTML5/ASCIIStringUnicodeCyclicIterator.h"

void* asciiStringUnicodeCyclicIterator_get(void* in_pIteratorState)
{
	ASCIIStringUnicodeCyclicIteratorState* pIteratorState = (ASCIIStringUnicodeCyclicIteratorState*) in_pIteratorState;

	/*
	 * Because of INV:ASCIIStringUnicodeCyclicIterator_h_26
	 * we may dereference the pointer
	 */
	assert(*pIteratorState->characterPointer >= 0);
	assert(*pIteratorState->characterPointer <= 127);

	// ASGN:ASCIIStringUnicodeCyclicIterator_c_17
	pIteratorState->codePoint = *pIteratorState->characterPointer;
	/*
	 * Follows from
	 * INV:ASCIIStringUnicodeCyclicIterator_h_27 and
	 * ASGN:ASCIIStringUnicodeCyclicIterator_c_17
	 */
	assert(pIteratorState->codePoint != 0);

	return &pIteratorState->codePoint;
}

IterateResult asciiStringUnicodeCyclicIterator_next(void* in_pIteratorState)
{
	ASCIIStringUnicodeCyclicIteratorState* pIteratorState = (ASCIIStringUnicodeCyclicIteratorState*) in_pIteratorState;

	assert(*pIteratorState->characterPointer >= 0);
	assert(*pIteratorState->characterPointer <= 127);

	// Follows from INV:ASCIIStringUnicodeCyclicIterator_h_27
	assert(*pIteratorState->characterPointer != 0);

	pIteratorState->characterPointer++;

	assert(*pIteratorState->characterPointer >= 0);
	assert(*pIteratorState->characterPointer <= 127);

	// CND:ASCIIStringUnicodeCyclicIterator_c_44
	if (*pIteratorState->characterPointer)
	{
		/*
		 * Because of
		 * CND:ASCIIStringUnicodeCyclicIterator_c_44
		 * the invariants
		 * INV:ASCIIStringUnicodeCyclicIterator_h_26 and
		 * INV:ASCIIStringUnicodeCyclicIterator_h_27
		 * are preserved.
		 */
		return IterateResultOK;
	}
	else
	{
		// ASGN:ASCIIStringUnicodeCyclicIterator_c_54
		pIteratorState->characterPointer = pIteratorState->beginOfString;
		/*
		 * Because of INV:ASCIIStringUnicodeCyclicIterator_h_23,
		 * INV:ASCIIStringUnicodeCyclicIterator_h_24 and
		 * ASGN:ASCIIStringUnicodeCyclicIterator_c_54
		 * the invariants
		 * INV:ASCIIStringUnicodeCyclicIterator_h_26 and
		 * INV:ASCIIStringUnicodeCyclicIterator_h_27
		 * are preserved.
		 */
		return IterateResultOverBoundary;
	}
}

SingleIterator asciiStringUnicodeCyclicIterator_create()
{
	SingleIterator out_si = { &asciiStringUnicodeCyclicIterator_get, &asciiStringUnicodeCyclicIterator_next };
	return out_si;
}

ASCIIStringUnicodeCyclicIteratorState asciiStringUnicodeCyclicIteratorState_create(char* string)
{
	ASCIIStringUnicodeCyclicIteratorState out_state;
	// INV:ASCIIStringUnicodeCyclicIterator_c:68; follows from PRE:ASCIIStringUnicodeCyclicIterator_h_38
	assert(string != NULL);
	// INV:ASCIIStringUnicodeCyclicIterator_c:70; follows from PRE:ASCIIStringUnicodeCyclicIterator_h_39
	assert(*string != 0);
	// ASGN:ASCIIStringUnicodeCyclicIterator_c:72
	out_state.beginOfString = string;
	// ASGN:ASCIIStringUnicodeCyclicIterator_c:74
	out_state.characterPointer = string;
	out_state.codePoint = 0; /* Setting it deliberately to an invalid value */
	/*
	 * From INV:ASCIIStringUnicodeCyclicIterator_c:68, 
	 * INV:ASCIIStringUnicodeCyclicIterator_c:70,
	 * ASGN:ASCIIStringUnicodeCyclicIterator_c:72 and
	 * ASGN:ASCIIStringUnicodeCyclicIterator_c:74
	 * we get
	 * POST:ASCIIStringUnicodeCyclicIterator_h_41, 
	 * POST:ASCIIStringUnicodeCyclicIterator_h_42,
	 * POST:ASCIIStringUnicodeCyclicIterator_h_43 and
	 * POST:ASCIIStringUnicodeCyclicIterator_h_44
	 */
	return out_state;
}
