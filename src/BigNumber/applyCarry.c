#include "BigNumber/BigInteger.h"
#include <stdlib.h>
#include <assert.h>

uint32_t applyCarry(UnsignedBigInteger* in_pInt, uint32_t in_c)
{
	assert(in_pInt->numberSize <= in_pInt->allocedLimbsCount);

	if (in_c != 0)
	{
		if (in_pInt->numberSize < SIZE_MAX-1)
		{
			if (in_pInt->numberSize < in_pInt->allocedLimbsCount)
			{
				in_pInt->limbs[in_pInt->numberSize] = in_c;
				in_pInt->numberSize++;
				return 0;
			}
			else
			{
				assert(in_pInt->numberSize == in_pInt->allocedLimbsCount);

				if (in_pInt->numberSize+1 <= SIZE_MAX/sizeof(uint32_t))
				{
					uint32_t* newLimbs = realloc(in_pInt->limbs, sizeof(uint32_t)*(in_pInt->numberSize+1));

					if (newLimbs != NULL)
					{
						in_pInt->limbs = newLimbs;
						in_pInt->limbs[in_pInt->numberSize] = in_c;

						in_pInt->allocedLimbsCount++;
						in_pInt->numberSize++;

						return 0;
					}
					else
					{
						return in_c;
					}
				}
				else
				{
					return in_c;
				}
			}
		}
		else
		{
			return in_c;
		}
	}
	else
	{
		return 0;
	}
}
