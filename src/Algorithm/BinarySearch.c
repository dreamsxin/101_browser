#include "Algorithm/BinarySearch.h"
#include <assert.h>

size_t binarySearch(const void* in_pToFind, const void* in_pFirst, size_t in_count, size_t in_size,
					IntermediateCompareResult (*in_pCompare)(const void* in_pToFind, const void* in_pDatum))
{
	size_t firstIndex;
	size_t  lastIndex;
	IntermediateCompareResult result;
	
	if (in_count == 0u)
		return (size_t) -1;

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
			/*
			 * Q: Where does this assertion come from?
			 * 
			 * A: The loop condition ensures (firstIndex < lastIndex). 
			 *    We have center = firstIndex + (lastIndex-firstIndex)/2.
			 *    
			 *    Let d = (lastIndex-firstIndex)/2 (using integer division)
			 *    So we have (this time using division in Q):
			 *    d <= (lastIndex-firstIndex)/2 (since the result of integer 
			 *    division is always smaller than with division in Q)
			 *    
			 *    From this we get 
			 *    center <= firstIndex+(lastIndex-firstIndex)/2 (again / division in Q).
			 *    This simplifies to 
			 *    center <= (lastIndex-firstIndex)/2 (again / division in Q).
			 *    Since firstIndex >= 0 we get:
			 *    center <= lastIndex/2 (again / division in Q)
			 *    For lastIndex >= 2 we obviously follow: center < lastIndex
			 * 
			 *    The case: lastIndex == 0 needs firstIndex == 0 (in which the loop
			 *    never gets entered)
			 *    In the remaining case: lastIndex == 1 we need firstIndex == 0
			 *    (since if firstIndex == 1 we would never enter the loop)
			 *    But then center = 0 => center < lastIndex
			 */
			assert(center < lastIndex);
			
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
