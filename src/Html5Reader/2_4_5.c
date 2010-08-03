#include "Html5Reader/2_4_5.h"
#include <assert.h>

uint8_t numberOfDaysInMonthOfYear(uint8_t month, uint32_t year)
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
			if (year % 400 == 0)
				return 29;
			else if (year % 4 == 0 && year % 100 != 0)
				return 28;
			else
				return 28;
		}
	}
}
