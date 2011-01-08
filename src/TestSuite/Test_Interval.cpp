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

#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Util/Interval.hpp"

void testInterval()
{
	// 0. tests concerning the correctness of the constructor of Interval
	Interval<unsigned int> i1 = createInterval<unsigned int>(34, 37);
	test(i1.const_x0()==34);
	test(i1.const_x1()==37);

	Interval<unsigned int> i2 = createInterval<unsigned int>(42, 29);
	test(i2.const_x0()==29);
	test(i2.const_x1()==42);

	// 1. tests where none of the intervals contains only one point
	i2 = createInterval<unsigned int>(20, 60);

	i1 =  createInterval<unsigned int>(0, 10);
	test(compareIntervals(i1, i2)==ExtendedCompareResultLess);

	i1 = createInterval<unsigned int>(0, 20);
	test(compareIntervals(i1, i2)==ExtendedCompareResultNotComparableLess);

	i1 = createInterval<unsigned int>(0, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultNotComparableLess);

	i1 = createInterval<unsigned int>(0, 60);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContains);

	i1 = createInterval<unsigned int>(0, 80);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContains);

	i1 = createInterval<unsigned int>(20, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContainedIn);

	i1 = createInterval<unsigned int>(20, 60);
	test(compareIntervals(i1, i2)==ExtendedCompareResultEqual);

	i1 = createInterval<unsigned int>(20, 80);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContains);

	i1 = createInterval<unsigned int>(30, 50);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContainedIn);

	i1 = createInterval<unsigned int>(40, 60);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContainedIn);

	i1 = createInterval<unsigned int>(40, 80);
	test(compareIntervals(i1, i2)==ExtendedCompareResultNotComparableGreater);

	i1 = createInterval<unsigned int>(60, 80);
	test(compareIntervals(i1, i2)==ExtendedCompareResultNotComparableGreater);

	i1 = createInterval<unsigned int>(70, 80);
	test(compareIntervals(i1, i2)==ExtendedCompareResultGreater);
	
	// 2. Tests where one of the intervals contains only one point

	// 2.1 the first interval contains only a single point
	i1 = createInterval<unsigned int>(20, 20);

	i2 = createInterval<unsigned int>(0, 10);
	test(compareIntervals(i1, i2)==ExtendedCompareResultGreater);

	i2 = createInterval<unsigned int>(0, 20);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContainedIn);

	i2 = createInterval<unsigned int>(0, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContainedIn);

	i2 = createInterval<unsigned int>(20, 20);
	test(compareIntervals(i1, i2)==ExtendedCompareResultEqual);

	i2 = createInterval<unsigned int>(20, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContainedIn);

	i2 = createInterval<unsigned int>(30, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultLess);

	// 2.2 the second interval contains only a single point
	i2 = createInterval<unsigned int>(20, 20);

	i1 = createInterval<unsigned int>(0, 10);
	test(compareIntervals(i1, i2)==ExtendedCompareResultLess);

	i1 = createInterval<unsigned int>(0, 20);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContains);

	i1 = createInterval<unsigned int>(0, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContains);

	// This test is redundant - but it won't mind
	i1 = createInterval<unsigned int>(20, 20);
	test(compareIntervals(i1, i2)==ExtendedCompareResultEqual);

	i1 = createInterval<unsigned int>(20, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultContains);

	i1 = createInterval<unsigned int>(30, 40);
	test(compareIntervals(i1, i2)==ExtendedCompareResultGreater);

	// 3. Now we compare an object with an interval
	unsigned int number;
	i1 = createInterval<unsigned int>(30, 40);

	number=10;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultLess);
	number=30;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultContainedIn);
	number=35;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultContainedIn);
	number=40;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultContainedIn);
	number=50;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultGreater);

	i1 = createInterval<unsigned int>(40, 40);
	number=10;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultLess);
	number=39;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultLess);
	number=40;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultEqual);
	number=41;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultGreater);
	number=50;
	test(compareElementInterval(&number, &i1) == IntermediateCompareResultGreater);
}
