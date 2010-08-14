#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Algorithm/BinarySearch.h"
#include "Util/Interval.hpp"

void testAlgorithm()
{
	{
		unsigned int number = 42;

		test(binarySearch(&number, NULL, 0, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == -1);
	}

	{
		Interval<unsigned int> intervals[1];
		intervals[0] = Interval<unsigned int>(5, 7);

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 1, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 5;
		test(binarySearch(&number, &intervals, 1, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 7;
		test(binarySearch(&number, &intervals, 1, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 10;
		test(binarySearch(&number, &intervals, 1, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}

	{
		Interval<unsigned int> intervals[2];
		intervals[0] = Interval<unsigned int>(5, 7);
		intervals[1] = Interval<unsigned int>(10, 15);

		unsigned int number;
		
		number = 0;
		test(binarySearch(&number, &intervals, 2, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 6;
		test(binarySearch(&number, &intervals, 2, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == 0);

		number = 8;
		test(binarySearch(&number, &intervals, 2, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);

		number = 12;
		test(binarySearch(&number, &intervals, 2, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == 1);

		number = 16;
		test(binarySearch(&number, &intervals, 2, sizeof Interval<unsigned int>, 
			&compareElementIntervalVoid<unsigned int>) == (size_t) -1);
	}
}