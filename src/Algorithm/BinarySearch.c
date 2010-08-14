#include "Algorithm/BinarySearch.h"
#include <assert.h>

size_t binarySearch(const void* in_pToFind, const void* in_pFirst, size_t in_count, size_t in_size,
					IntermediateCompareResult (*in_pCompare)(const void* in_pToFind, const void* in_pDatum))
{
	size_t firstIndex;
	size_t  lastIndex;
	IntermediateCompareResult result;
	
	if (in_count == 0)
		return -1;

	firstIndex = 0;
	lastIndex = in_count-1;

	assert(firstIndex <= lastIndex);

	while (firstIndex < lastIndex)
	{
		/*
		 * I claim that this is equal to (firstIndex+lastIndex)/2
		 * if we had enough precission to represent lastIndex+firstIndex.
		 *
		 * Proof:
		 * Case 1: (firstIndex%2 == 0 and lastIndex%2 == 0) or 
		 *         (firstIndex%2 == 1 and lastIndex%2 == 1)
		 * 
		 *         In these cases all sums and differences are even
		 *         and by this divisable by 2
		 *
		 * Case 2: (firstIndex%2 == 0 and lastIndex%2 == 1)
		 *         Then (all computations in high precission with integer division)
		 *              (firstIndex+lastIndex)/2 
		 *            = (firstIndex+lastIndex-1)/2
		 *            = firstIndex+(lastIndex-firstIndex-1)/2
		 *            = firstIndex+(lastIndex-firstIndex)/2
		 *
		 * Case 3: (firstIndex%2 == 1 and lastIndex%2 == 0)
		 *         Then (all computations in high precission with integer division)
		 *              (firstIndex+lastIndex)/2 
		 *            = (firstIndex+lastIndex-1)/2
		 *            = ((firstIndex-1) + lastIndex)/2
		 *            = (firstIndex-1) + (lastIndex-(firstIndex-1))/2
		 *            = firstIndex-1 + (lastIndex-firstIndex+1)/2
		 *            = firstIndex-1 + (lastIndex-firstIndex)/2+1
		 *            = firstIndex + (lastIndex-firstIndex)/2
		 */
		size_t center = firstIndex + (lastIndex-firstIndex)/2;

		result = (*in_pCompare)(in_pToFind, ((const uint8_t*) in_pFirst)+in_size*center);

		if (result == IntermediateCompareResultGreater)
		{
			if (center == lastIndex)
				return -1;
			else
				firstIndex = center+1;
		}
		else if (result == IntermediateCompareResultLess)
		{
			if (center == 0)
				return -1;
			else
				lastIndex = center-1;
		}
		else
		{
			return center;
		}
	}

	if (firstIndex > lastIndex)
		return -1;

	result = (*in_pCompare)(in_pToFind, ((const uint8_t*) in_pFirst)+in_size*firstIndex);

	if (result == IntermediateCompareResultGreater || result == IntermediateCompareResultLess)
	{
		return -1;
	}
	else
	{
		return firstIndex;
	}
}
