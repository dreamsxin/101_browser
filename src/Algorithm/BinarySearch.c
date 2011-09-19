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

#include "Algorithm/BinarySearch.h"
#include "MiniStdlib/cstdint.h"
#include <assert.h>

bool binarySearch(const void* in_pToFind, const void* in_pFirst, size_t in_count, size_t in_size,
	IntermediateCompareResult (*in_pCompare)(const void* in_pToFind, const void* in_pDatum), 
	size_t *out_pIndex)
{
	size_t firstIndex;
	size_t  lastIndex;
	IntermediateCompareResult result;
	
	if (in_count == 0u)
	{
		if (out_pIndex)
			*out_pIndex = 0;

		return false;
	}

	firstIndex = 0;
	lastIndex = in_count-1;

	assert(lastIndex != (size_t) -1);
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
			{
				if (out_pIndex)
					*out_pIndex = 0;

				return false;
			}
			else
				lastIndex = center-1;
		}
		else
		{
			if (out_pIndex)
				*out_pIndex = center;

			return true;
		}
	}

	/*
	* Q: Can this case even happen?
	* A: Yes, it can. Let the array consist of
	*    {0, 1, 3, 4} and we look for 2.
	*    So firstIndex == 0 and lastIndex == 3. Then center == 1.
	*
	*    Since 2 > 1 we set
	*    firstIndex = 2 (lastIndex stays 3).
	*
	*    The new center is 2.
	*
	*    Next we see 2 < 3
	*    So we set lastIndex = center - 1 ( = 1) (firstIndex stays 2)
	*    and get a case where this condition is satisfied.
	*/
	if (firstIndex > lastIndex)
	{
		/*
		* We could only get in this case when we got IntermediateCompareResultLess
		* as result.
		* Then we have lastIndex = center-1 and
		* toFind < array[center].
		* So center (=lastIndex + 1) is the correct value.
		*/
		if (out_pIndex)
			*out_pIndex = lastIndex+1;

		return false;
	}

	result = (*in_pCompare)(in_pToFind, ((const uint8_t*) in_pFirst)+in_size*firstIndex);

	assert(firstIndex == lastIndex);

	if (result == IntermediateCompareResultGreater)
	{
		if (out_pIndex)
			*out_pIndex = firstIndex+1;

		return false;
	}
	else if (result == IntermediateCompareResultLess)
	{
		if (out_pIndex)
			*out_pIndex = firstIndex;

		return false;
	}
	else
	{
		if (out_pIndex)
			*out_pIndex = firstIndex;

		return true;
	}
}
