#include "BasicDataStructures/BigNumber/BigInteger.h"

#include <cassert>
#include <climits>

bool isZero(UnsignedBigInteger const * const in_pcInt)
{
	return in_pcInt->numberSize == 0;
}

bool inc(UnsignedBigInteger* in_pInt)
{
	assert(in_pInt->numberSize <= in_pInt->allocedLimbsCount);

	/* 
	 * The reason why we choose this value is:
	 * if we have in_pInt->numberSize <= SIZE_MAX-1
	 * we get in_pInt->allocedLimbsCount<=SIZE_MAX
	 * i. e. we may run through it by a normal for loop
	 */
	if (in_pInt->numberSize >= SIZE_MAX)
	{
		return false;
	}

	// > can't happen
	if (in_pInt->numberSize == in_pInt->allocedLimbsCount)
	{
		unsigned long* newLimbs = (unsigned long*) 
			realloc(in_pInt->limbs, in_pInt->numberSize+1);
		if (newLimbs == NULL)
			return false;

		in_pInt->limbs = newLimbs;
		in_pInt->allocedLimbsCount = in_pInt->numberSize+1;
	}

	for (size_t currentPos = 0; currentPos < in_pInt->numberSize; currentPos++)
	{
		in_pInt->limbs[currentPos]++;

		if (in_pInt->limbs[currentPos]!=0)
		{
			return true;
		}
	}

	/* 
	 * When we got here we have an additional carrier 
	 * to handle
     *
	 * This is correct since we allocated 
	 * in_pInt->numberSize+1 limbs
	 */
	in_pInt->limbs[in_pInt->numberSize]=1;
	in_pInt->numberSize++;

	return true;
}

bool dec(UnsignedBigInteger* in_pInt)
{
	assert(in_pInt->numberSize <= in_pInt->allocedLimbsCount);
	assert(in_pInt->numberSize <= SIZE_MAX-1);

	if (isZero(in_pInt))
		return false;

	for (size_t currentPos = 0; currentPos < in_pInt->numberSize; currentPos++)
	{
		in_pInt->limbs[currentPos]--;

		if (in_pInt->limbs[currentPos]!=ULONG_MAX)
		{
			return true;
		}
	}

	/* 
	 * The highest limb could have gone to 0. 
	 * This is exactly the case if we got here.
	 * So we can free 1 limb (which we will do to
	 * avoid unnecessary memory usage).
	 */
	assert(in_pInt->limbs[in_pInt->numberSize-1]==0);

	size_t newNumberSize = in_pInt->numberSize;

	while (in_pInt->limbs[newNumberSize-1]==0)
		newNumberSize--;

	unsigned long* newLimbs = (unsigned long*) 
		realloc(in_pInt->limbs, newNumberSize);

	assert(newLimbs);

	if (newLimbs == NULL)
		return false;

	in_pInt->limbs = newLimbs;
	in_pInt->allocedLimbsCount = newNumberSize;

	return true;
}
