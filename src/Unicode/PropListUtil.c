#include <assert.h>
#include "Unicode/PropList.h"
#include "MiniStdlib/safe_free.h"

void freeIntervalsFromPropList(void* in_ppIntervals)
{
	safe_free(in_ppIntervals);
}

uint8_t readHexDigit(char token)
{
	if (token >= '0' && token <= '9')
	{
		return (token - '0');
	}
	else if (token >= 'A' && token <= 'F')
	{
		return 0xA + (token - 'A');
	}
	else
	{
		return (uint8_t) -1;
	}
}
