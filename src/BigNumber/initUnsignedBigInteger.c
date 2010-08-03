#include "BigNumber/BigInteger.h"
#include <assert.h>
#include <stdlib.h>

uint8_t initUnsignedBigInteger(UnsignedBigInteger* in_pNumber, 
							   uint32_t* limbs, size_t limbsCount)
{
	size_t idx;

	assert(limbsCount < SIZE_MAX/sizeof(uint32_t));

	{
		uint32_t* allocLimbs = (uint32_t*) malloc(sizeof(uint32_t) * limbsCount);

		if (allocLimbs == NULL)
			return 0;
		else
			in_pNumber->limbs = allocLimbs;
	}

	in_pNumber->allocedLimbsCount = limbsCount;
	in_pNumber->numberSize = 0;

	for (idx = 0; idx < limbsCount; idx++)
	{
		if (limbs[idx] != 0)
			in_pNumber->numberSize = idx+1;

		in_pNumber->limbs[idx] = limbs[idx];
	}

	return 1;
}
