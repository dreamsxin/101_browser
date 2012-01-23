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

#include "HTML5/2_5_4_1.h"

#include <assert.h>
#include "HTML5/2_5_1.h"
#include "Unicode/Unicode.h"

ReadResult parseNonNegativeInteger(SingleIterator in_iterator, void* in_iteratorState, 
							   UnsignedBigInteger* in_pUnsignedInteger)
{
	UnicodeCodePoint *pUnicodeCodePoint;
	UnsignedBigInteger out_unsignedBigInteger;
	bool lResult = skipWhitespace(in_iterator, in_iteratorState);

	if (!lResult)
		return ReadResultPrematureEndOfStream;

	pUnicodeCodePoint = (UnicodeCodePoint*) in_iterator.mpfGet(in_iteratorState);

	assert(pUnicodeCodePoint != NULL);

	// this will never get called - but nevertheless we test it for security
	if (pUnicodeCodePoint == NULL)
		return ReadResultSomeError;

	if (*pUnicodeCodePoint == '+')
	{
		if (in_iterator.mpfIterate(in_iteratorState) != IterateResultOK)
			return ReadResultPrematureEndOfStream;
	}

	lResult = initUnsignedBigIntegerUC(&out_unsignedBigInteger, 0);

	assert(lResult);
	if (!lResult)
		return ReadResultAllocationFailure;

	do
	{
		pUnicodeCodePoint = (UnicodeCodePoint*) in_iterator.mpfGet(in_iteratorState);;

		if (pUnicodeCodePoint == NULL)
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ReadResultSomeError;
		}

		if (*pUnicodeCodePoint < '0' || *pUnicodeCodePoint > '9')
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ReadResultInvalidData;
		}

		if (mulUBUC(&out_unsignedBigInteger, 10) != 0)
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ReadResultAllocationFailure;
		}

		if (addUBUC(&out_unsignedBigInteger, *pUnicodeCodePoint-'0') != 0)
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ReadResultAllocationFailure;
		}

		if (in_iterator.mpfIterate(in_iteratorState) != IterateResultOK)
		{
			*in_pUnsignedInteger = out_unsignedBigInteger;
			return ReadResultOK;
		}
	} while (1);
}
