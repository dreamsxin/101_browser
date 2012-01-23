/*
 * Copyright 2008-2012 Wolfgang Keller
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
#include "Unicode/Unicode.h"

void testUnicode()
{
#if 0
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

	test(readPropList(propListFile, "Dash", &pIntervals, &intervalsCount));
	test(intervalsCount == 19);

	test(readPropList(propListFile, "Noncharacter_Code_Point", &pIntervals, &intervalsCount));
	test(intervalsCount == 18);
#endif
}
