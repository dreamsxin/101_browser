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

#include "HTML5/2_3.h"
#include <stdlib.h>
#include <assert.h>

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
		UnicodeCodePoint* cp0 = (UnicodeCodePoint*) (*in_it.mpfGet)(in_iteratorState0);
		UnicodeCodePoint* cp1 = (UnicodeCodePoint*) (*in_it.mpfGet)(in_iteratorState1);

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

			if (!compResult)
				return false;
			else
			{
				IterateResult ir0 = (*in_it.mpfIterate)(in_iteratorState0);
				IterateResult ir1 = (*in_it.mpfIterate)(in_iteratorState1);

				if (ir0 == IterateResultOK && ir1 != IterateResultOK)
					return false;
				else if (ir0 != IterateResultOK && ir1 == IterateResultOK)
					return false;
				else if (ir0 != IterateResultOK && ir1 != IterateResultOK)
					return true;
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
	if (in_p>=0x0041 && in_p<=0x005A)
		in_p += (0x0061-0x0041);

	return in_p;
}

UnicodeCodePoint tokenToASCIIUppercase(UnicodeCodePoint in_p)
{
	if (in_p>=0x0061 && in_p<=0x007A)
		in_p -= (0x0061-0x0041);
	
	return in_p;
}

void universalConvertString(char* in_string, UnicodeCodePoint (*in_func)(UnicodeCodePoint))
{
	while (*in_string)
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
		UnicodeCodePoint* pPatternPoint = (UnicodeCodePoint*) (*in_it.mpfGet)(in_patternState);
		UnicodeCodePoint* pStringPoint = (UnicodeCodePoint*) (*in_it.mpfGet)(in_stringState);
		IterateResult irPattern;
		IterateResult irString;

		if (pPatternPoint == NULL)
			return true;

		assert(pPatternPoint != NULL);

		if (pStringPoint == NULL)
			return false;

		assert(pStringPoint != NULL);

		if (*pPatternPoint != *pStringPoint)
			return false;

		irPattern = (*in_it.mpfIterate)(in_patternState);
		irString = (*in_it.mpfIterate)(in_stringState);

		if (IterateResultOK == irPattern && IterateResultOK != irString)
			return false;
		else if (IterateResultOK != irPattern)
			return true;
	}
}
