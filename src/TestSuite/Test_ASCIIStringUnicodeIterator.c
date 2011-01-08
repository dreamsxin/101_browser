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
