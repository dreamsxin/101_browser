#include "HTML5/2_3.h"
#include "assert.h"

bool compareTokensCaseSensitive(UnicodeCodePoint p1, UnicodeCodePoint p2)
{
	return (p1 == p2);
}

bool compareTokensASCIICaseInsensitive(UnicodeCodePoint p1, UnicodeCodePoint p2)
{
	if (p1>=0x0061 && p1<=0x007A)
		p1 -= (0x0061-0x0041);

	if (p2>=0x0061 && p2<=0x007A)
		p2 -= (0x0061-0x0041);

	return p1 == p2;
}

bool universalCompare(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1, bool (*in_cmpFunc)(UnicodeCodePoint, UnicodeCodePoint))
{
	while (1)
	{
		UnicodeCodePoint* cp0 = (*in_it.mpfGet)(in_iteratorState0);
		UnicodeCodePoint* cp1 = (*in_it.mpfGet)(in_iteratorState1);

		if ((cp0 == NULL && cp1 != NULL) || (cp0 != NULL && cp1 == NULL))
		{
			return false;
		}
		else if (cp0 == NULL && cp1 == NULL)
		{
			return true;
		}
		else
		{
			bool compResult;
			assert(cp0 != NULL);
			assert(cp1 != NULL);

			compResult = (*in_cmpFunc)(*cp0, *cp1);

			if (compResult)
				return true;
			else
			{
				(*in_it.mpfIterate)(in_iteratorState0);
				(*in_it.mpfIterate)(in_iteratorState1);
			}
		}
	}
}

bool compareStringsCaseSensitive(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1)
{
	return universalCompare(in_it, in_iteratorState0, in_iteratorState1, &compareTokensCaseSensitive);
}

bool compareStringsASCIICaseInsensitive(SingleIterator in_it, void* in_iteratorState0, void* in_iteratorState1)
{
	return universalCompare(in_it, in_iteratorState0, in_iteratorState1, &compareTokensASCIICaseInsensitive);
}

UnicodeCodePoint tokenToASCIILowercase(UnicodeCodePoint in_p)
{
	if (in_p>=0x0061 && in_p<=0x007A)
		in_p -= (0x0061-0x0041);
	
	return in_p;
}

UnicodeCodePoint tokenToASCIIUppercase(UnicodeCodePoint in_p)
{
	if (in_p>=0x0041 && in_p<=0x005A)
		in_p += (0x0061-0x0041);

	return in_p;
}

void universalConvertString(char* in_string, UnicodeCodePoint (*in_func)(UnicodeCodePoint))
{
	while (in_string != NULL)
	{
		UnicodeCodePoint p = (*in_func)((unsigned char) *in_string);
		*in_string = (unsigned char) p;
		in_string++;
	}
}

void convertStringToASCIILowercase(char* in_string)
{
	universalConvertString(in_string, &tokenToASCIILowercase);
}

void convertStringToASCIIUppercase(char* in_string)
{
	universalConvertString(in_string, &tokenToASCIIUppercase);
}

bool prefixMatch(SingleIterator in_it, void* in_patternState, void* in_stringState)
{
	while (true)
	{
		UnicodeCodePoint* pPatternPoint = (*in_it.mpfGet)(in_patternState);
		UnicodeCodePoint* pStringPoint = (*in_it.mpfGet)(in_stringState);

		if (pPatternPoint == NULL)
			return true;

		assert(pPatternPoint != NULL);

		if (pStringPoint == NULL)
			return false;

		assert(pStringPoint != NULL);

		if (*pPatternPoint != *pStringPoint)
			return false;

		(*in_it.mpfIterate)(in_patternState);
		(*in_it.mpfIterate)(in_stringState);
	}
}
