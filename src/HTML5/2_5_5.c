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

#include "HTML5/2_5_5.h"
#include <assert.h>

uint8_t numberOfDaysInMonthOfYear(uint8_t month, UnsignedBigInteger year)
{
	assert(month >= 1);
	assert(month <= 12);

	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	case 2:
		{
			if (modUBUC(&year, 400) == 0)
				return 29;
			else if (modUBUC(&year, 4) == 0 && modUBUC(&year, 100) != 0)
				return 29;
			else
				return 28;
		}
	default:
		return 0;
	}
}

bool isDigit(UnicodeCodePoint in_p)
{
	return (in_p >= 0x0030  /* DIGIT ZERO */
		&& in_p <= 0x0039); /* DIGIT NINE */
}
