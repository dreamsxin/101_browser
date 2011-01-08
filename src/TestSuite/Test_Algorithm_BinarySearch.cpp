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
	{
		unsigned int number = 42;

		test(binarySearch(&number, NULL, 0, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == -1);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7} };

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 5;
		test(binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 7;
		test(binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 10;
		test(binarySearch(&number, &intervals, 1, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15} };

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 6;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 8;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 12;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 1);

		number = 16;
		test(binarySearch(&number, &intervals, 2, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15}, {20, 25} };

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 6;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 8;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 12;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 1);

		number = 16;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 22;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 2);

		number = 27;
		test(binarySearch(&number, &intervals, 3, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15}, {20, 25}, {30, 35} };

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 6;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 8;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 12;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 1);

		number = 16;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 22;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 2);

		number = 27;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 32;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 3);

		number = 37;
		test(binarySearch(&number, &intervals, 4, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}

	{
		Interval<unsigned int> intervals[] = { {5, 7}, {10, 15}, {20, 25}, {30, 35}, {40, 45} };

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 6;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 8;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 12;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 1);

		number = 16;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 22;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 2);

		number = 27;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 32;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 3);

		number = 37;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 42;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == 4);

		number = 47;
		test(binarySearch(&number, &intervals, 5, sizeof(Interval<unsigned int>), 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}
}
