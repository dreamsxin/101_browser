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
