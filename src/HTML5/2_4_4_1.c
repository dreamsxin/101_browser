#include "HTML5/2_4_4_1.h"

#include <assert.h>
#include "HTML5/2_4_1.h"
#include "Util/Unicode.h"

enum Result parseNonNegativeInteger(SingleIterator in_iterator, void* in_iteratorState, 
							   UnsignedBigInteger* in_pUnsignedInteger)
{
	UnicodeCodePoint *pUnicodeCodePoint;
	UnsignedBigInteger out_unsignedBigInteger;
	bool lResult = skipWhitespace(in_iterator, in_iteratorState);

	if (!lResult)
		return ResultError;

	pUnicodeCodePoint = (UnicodeCodePoint*) in_iterator.mpfGet(in_iteratorState);

	assert(pUnicodeCodePoint != NULL);

	// this will never get called - but nevertheless we test it for security
	if (pUnicodeCodePoint == NULL)
		return ResultError;

	if (*pUnicodeCodePoint == '+')
	{
		if (in_iterator.mpfIterate(in_iteratorState) != IterateResultOK)
			return ResultError;
	}

	lResult = initUnsignedBigIntegerUC(&out_unsignedBigInteger, 0);

	assert(lResult);
	if (!lResult)
		return ResultError;

	do
	{
		pUnicodeCodePoint = (UnicodeCodePoint*) in_iterator.mpfGet(in_iteratorState);;

		if (pUnicodeCodePoint == NULL)
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ResultError;
		}

		if (*pUnicodeCodePoint < '0' || *pUnicodeCodePoint > '9')
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ResultError;
		}

		if (mulUBUC(&out_unsignedBigInteger, 10) != 0)
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ResultAllocationFailure;
		}

		if (addUBUC(&out_unsignedBigInteger, *pUnicodeCodePoint-'0') != 0)
		{
			freeUnsignedBigInteger(&out_unsignedBigInteger);
			return ResultAllocationFailure;
		}

		if (in_iterator.mpfIterate(in_iteratorState) != IterateResultOK)
		{
			*in_pUnsignedInteger = out_unsignedBigInteger;
			return ResultOK;
		}
	} while (1);
}
