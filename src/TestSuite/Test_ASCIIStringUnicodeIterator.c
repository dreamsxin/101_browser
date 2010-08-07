#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "HTML5/ASCIIStringUnicodeIterator.h"

void testASCIIStringUnicodeIterator()
{
	SingleIterator it = asciiStringUnicodeIterator_create();
	UnicodeCodePoint* pu;

	/* empty string */
	ASCIIStringUnicodeIteratorState state = asciiStringUnicodeIteratorState_create("");
	test((*it.mpfGet)(&state) == NULL);
	test((*it.mpfIterate)(&state) == IterateResultToInvalidState);

	/* string of single character */
	state = asciiStringUnicodeIteratorState_create("a");
	pu = (*it.mpfGet)(&state);
	test(pu != NULL);
	if (pu != NULL)
	{
		test (*pu == 'a');
	}
	test((*it.mpfIterate)(&state) == IterateResultToInvalidState);
	test((*it.mpfGet)(&state) == NULL);
	test((*it.mpfIterate)(&state) == IterateResultToInvalidState);

	/* string of multiple characters */
	state = asciiStringUnicodeIteratorState_create("asdf");
	pu = (*it.mpfGet)(&state);
	test(pu != NULL);
	if (pu != NULL)
	{
		test (*pu == 'a');
	}
	test((*it.mpfIterate)(&state) == IterateResultOK);
	pu = (*it.mpfGet)(&state);
	test(pu != NULL);
	if (pu != NULL)
	{
		test (*pu == 's');
	}
	test((*it.mpfIterate)(&state) == IterateResultOK);
	pu = (*it.mpfGet)(&state);
	test(pu != NULL);
	if (pu != NULL)
	{
		test (*pu == 'd');
	}
	test((*it.mpfIterate)(&state) == IterateResultOK);
	pu = (*it.mpfGet)(&state);
	test(pu != NULL);
	if (pu != NULL)
	{
		test (*pu == 'f');
	}
	test((*it.mpfIterate)(&state) == IterateResultToInvalidState);
	test((*it.mpfGet)(&state) == NULL);
	test((*it.mpfIterate)(&state) == IterateResultToInvalidState);
}
