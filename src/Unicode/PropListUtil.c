#include <assert.h>
#include "Unicode/PropList.h"
#include "MiniStdlib/safe_free.h"

void freeIntervalsFromPropList(void* in_ppIntervals)
{
	safe_free(in_ppIntervals);
}

/*!
 * Precondition: (token >= '0' && token <= '9') || (token >= 'A' && token <= 'F')
 */
uint8_t readHexDigit(char token)
{
	if (token >= '0' && token <= '9')
	{
		return (token-'0');
	}
	else
	{
		assert(token >= 'A' && token <= 'F');

		return 10+(token - 'A');
	}
}
