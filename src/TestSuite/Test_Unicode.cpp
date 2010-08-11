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

	test(readPropList(propListFile, "Dash", &pIntervals));
	freeIntervalsFromPropList(&pIntervals);
}
