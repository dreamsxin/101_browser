#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "Util/Interval.hpp"
#include "Util/Unicode.h"
#include "Unicode/PropList.h"
#include <cstdio>
#include <cstdlib>

void testUnicode()
{
	FILE* propListFile = fopen("data/Unicode/PropList.txt", "rb");

	test(propListFile != NULL);

	if (propListFile == NULL)
		return;

	Interval<UnicodeCodePoint>* pIntervals;
	size_t intervalsCount;

	test(readPropList(propListFile, "White_Space", &pIntervals, &intervalsCount));
	test(intervalsCount == 12);
	freeIntervalsFromPropList(&pIntervals);

	test(readPropList(propListFile, "Dash", &pIntervals, &intervalsCount));
	test(intervalsCount == 19);
	test(pIntervals[0].const_x0() == 0x002D);
	test(pIntervals[0].const_x1() == 0x002D);
	test(pIntervals[5].const_x0() == 0x2010);
	test(pIntervals[5].const_x1() == 0x2015);
	test(pIntervals[18].const_x0() == 0xFF0D);
	test(pIntervals[18].const_x1() == 0xFF0D);
	freeIntervalsFromPropList(&pIntervals);

	test(readPropList(propListFile, "Dash", &pIntervals, &intervalsCount));
	test(intervalsCount == 19);
	freeIntervalsFromPropList(&pIntervals);

	test(readPropList(propListFile, "Noncharacter_Code_Point", &pIntervals, &intervalsCount));
	test(intervalsCount == 18);
	freeIntervalsFromPropList(&pIntervals);
}
