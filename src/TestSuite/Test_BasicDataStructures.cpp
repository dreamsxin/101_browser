#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Util/Interval.hpp"

void testBasicDataStructures()
{
	// 0. tests concerning the correctness of the constructor of Interval
	Interval<unsigned int> i1(34, 37);
	test(i1.const_x0()==34);
	test(i1.const_x1()==37);

	Interval<unsigned int> i2(42, 29);
	test(i2.const_x0()==29);
	test(i2.const_x1()==42);

	// 1. tests where none of the intervals contains only one point
	i2=Interval<unsigned int>(20, 60);

	i1=Interval<unsigned int>(0, 10);
	test(compareIntervals(i1, i2)==Less);

	i1=Interval<unsigned int>(0, 20);
	test(compareIntervals(i1, i2)==NotComparableLess);

	i1=Interval<unsigned int>(0, 40);
	test(compareIntervals(i1, i2)==NotComparableLess);

	i1=Interval<unsigned int>(0, 60);
	test(compareIntervals(i1, i2)==Contains);

	i1=Interval<unsigned int>(0, 80);
	test(compareIntervals(i1, i2)==Contains);

	i1=Interval<unsigned int>(20, 40);
	test(compareIntervals(i1, i2)==ContainedIn);

	i1=Interval<unsigned int>(20, 60);
	test(compareIntervals(i1, i2)==Equal);

	i1=Interval<unsigned int>(20, 80);
	test(compareIntervals(i1, i2)==Contains);

	i1=Interval<unsigned int>(30, 50);
	test(compareIntervals(i1, i2)==ContainedIn);

	i1=Interval<unsigned int>(40, 60);
	test(compareIntervals(i1, i2)==ContainedIn);

	i1=Interval<unsigned int>(40, 80);
	test(compareIntervals(i1, i2)==NotComparableGreater);

	i1=Interval<unsigned int>(60, 80);
	test(compareIntervals(i1, i2)==NotComparableGreater);

	i1=Interval<unsigned int>(70, 80);
	test(compareIntervals(i1, i2)==Greater);
	
	// 2. Tests where one of the intervals contains only one point

	// 2.1 the first interval contains only a single point
	i1=Interval<unsigned int>(20, 20);

	i2=Interval<unsigned int>(0, 10);
	test(compareIntervals(i1, i2)==Greater);

	i2=Interval<unsigned int>(0, 20);
	test(compareIntervals(i1, i2)==ContainedIn);

	i2=Interval<unsigned int>(0, 40);
	test(compareIntervals(i1, i2)==ContainedIn);

	i2=Interval<unsigned int>(20, 20);
	test(compareIntervals(i1, i2)==Equal);

	i2=Interval<unsigned int>(20, 40);
	test(compareIntervals(i1, i2)==ContainedIn);

	i2=Interval<unsigned int>(30, 40);
	test(compareIntervals(i1, i2)==Less);

	// 2.2 the second interval contains only a single point
	i2=Interval<unsigned int>(20, 20);

	i1=Interval<unsigned int>(0, 10);
	test(compareIntervals(i1, i2)==Less);

	i1=Interval<unsigned int>(0, 20);
	test(compareIntervals(i1, i2)==Contains);

	i1=Interval<unsigned int>(0, 40);
	test(compareIntervals(i1, i2)==Contains);

	// This test is redundant - but it won't mind
	i1=Interval<unsigned int>(20, 20);
	test(compareIntervals(i1, i2)==Equal);

	i1=Interval<unsigned int>(20, 40);
	test(compareIntervals(i1, i2)==Contains);

	i1=Interval<unsigned int>(30, 40);
	test(compareIntervals(i1, i2)==Greater);
}
