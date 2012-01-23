/*
* Copyright 2012 Wolfgang Keller
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

#include "Util/Interval.hpp"
#include "Unicode/Unicode.h"

const Interval<UnicodeCodePoint> White_SpaceIntervals[] = {
	{0x0009, 0x000D}, 
	{0x0020, 0x0020}, 
	{0x0085, 0x0085}, 
	{0x00A0, 0x00A0}, 
	{0x1680, 0x1680}, 
	{0x180E, 0x180E}, 
	{0x2000, 0x200A}, 
	{0x2028, 0x2028}, 
	{0x2029, 0x2029}, 
	{0x202F, 0x202F}, 
	{0x205F, 0x205F}, 
	{0x3000, 0x3000}
};

const void* getWhite_SpaceIntervals()
{
	return White_SpaceIntervals;
}

size_t getWhite_SpaceIntervalsCount()
{
	return sizeof(White_SpaceIntervals) / sizeof(Interval<UnicodeCodePoint>);
}
