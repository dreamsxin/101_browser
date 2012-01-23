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

#include "HTML5/2_5_1.h"
#include "Algorithm/BinarySearch.h"
#include "Util/Interval.hpp"
#include "Unicode/Unicode.h"

const Interval<UnicodeCodePoint> spaceCharacterIntervals[3] = 
{
	{ 0x0009, 0x000A},
	{ 0x000C, 0x000D},
	{ 0x0020, 0x0020}
};

bool isSpaceCharacter(UnicodeCodePoint in_p)
{
	return binarySearch(&in_p, spaceCharacterIntervals, 3, 
		sizeof(Interval<UnicodeCodePoint>), 
		&compareElementIntervalVoid<UnicodeCodePoint>, NULL);
}

const Interval<UnicodeCodePoint> ASCIICharacterIntervals[3] = 
{
	{ 0x0030, 0x0039},
	{ 0x0041, 0x005A},
	{ 0x0061, 0x007A}
};

bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p)
{
	return binarySearch(&in_p, ASCIICharacterIntervals, 3, 
		sizeof(Interval<UnicodeCodePoint>), 
		&compareElementIntervalVoid<UnicodeCodePoint>, NULL);
}

bool isWhite_SpaceCharacter(UnicodeCodePoint in_p)
{
	return binarySearch(&in_p, getWhite_SpaceIntervals(), getWhite_SpaceIntervalsCount(), 
		sizeof(Interval<UnicodeCodePoint>), 
		&compareElementIntervalVoid<UnicodeCodePoint>, NULL);
}
