#include "HTML5/2_4_5.h"
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
	}
}

bool isDigit(UnicodeCodePoint in_p)
{
	return (in_p >= '0' && in_p <= '9');
}