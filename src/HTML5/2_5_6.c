/*
* Copyright 2011-2012 Wolfgang Keller
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

#include "HTML5/2_5_6.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/MTAx_cstdlib.h"
#include "MiniStdlib/HexDigit.h"
#include "Unicode/Unicode.h"

ReadResult parseSimpleColor(SingleIterator in_iterator, void* in_iteratorState, 
SimpleColor* in_pSimpleColor)
{
	UnicodeCodePoint *pCp = (UnicodeCodePoint*) (*in_iterator.mpfGet)(in_iteratorState);
	uint8_t idx;
	uint8_t colorBytes[6];

	if (NULL == pCp)
		return ReadResultPrematureEndOfStream;

	if (*pCp != '#')
		return ReadResultInvalidData;

	for (idx = 0; idx < 6; idx++)
	{
		if (in_iterator.mpfIterate(in_iteratorState) != IterateResultOK)
			return ReadResultPrematureEndOfStream;

		pCp = (UnicodeCodePoint*) (*in_iterator.mpfGet)(in_iteratorState);
		if (NULL == pCp)
		{
			return ReadResultInternalInconsistency;
		}

		if (!convertHexDigitToNumber(*pCp, colorBytes+idx, true, true))
			return ReadResultInvalidData;
	}

	if (in_iterator.mpfIterate(in_iteratorState) == IterateResultOK)
		return ReadResultOverdueEndOfStream;

	in_pSimpleColor->red   = colorBytes[0]*16+colorBytes[1];
	in_pSimpleColor->green = colorBytes[2]*16+colorBytes[3];
	in_pSimpleColor->blue  = colorBytes[4]*16+colorBytes[5];

	return ReadResultOK;
}
