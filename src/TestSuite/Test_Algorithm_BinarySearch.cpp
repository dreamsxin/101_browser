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

#include "TestSuite/TestAlgorithm.h"
#include "Algorithm/BinarySearch.h"
#include "Util/Interval.hpp"

void testAlgorithm_BinarySearch()
{
	size_t index;
	unsigned int number;
	
	{
		number = 42;

		test(!binarySearch(&number, NULL, 0, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7} };
		
		number = 0;
		test(!binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 5;
		test(binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 7;
		test(binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 10;
		test(!binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15} };
		
		number = 0;
		test(!binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 6;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 8;
		test(!binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 12;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 16;
		test(!binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15}, {20, 25} };
		
		number = 0;
		test(!binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 6;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 8;
		test(!binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 12;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 16;
		test(!binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);

		number = 22;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);

		number = 27;
		test(!binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(3 == index);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15}, {20, 25}, {30, 35} };
		
		number = 0;
		test(!binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 6;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 8;
		test(!binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 12;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 16;
		test(!binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);

		number = 22;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);

		number = 27;
		test(!binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(3 == index);

		number = 32;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(3 == index);

		number = 37;
		test(!binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(4 == index);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15}, {20, 25}, {30, 35}, {40, 45} };
		
		number = 0;
		test(!binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 6;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(0 == index);

		number = 8;
		test(!binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 12;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(1 == index);

		number = 16;
		test(!binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);

		number = 22;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(2 == index);

		number = 27;
		test(!binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index) );
		test(3 == index);

		number = 32;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(3 == index);

		number = 37;
		test(!binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(4 == index);

		number = 42;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(4 == index);

		number = 47;
		test(!binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>, &index));
		test(5 == index);
	}
}
