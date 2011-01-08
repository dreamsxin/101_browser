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

#ifndef _Comparator_h
#define _Comparator_h

#include "Algorithm/CompareResult.h"

template <typename Type> SimpleCompareResult simpleCompare(Type in_a, Type in_b)
{
	if (in_a<in_b)
		return SimpleCompareResultLess;
	if (in_a>in_b)
		return SimpleCompareResultGreater;

	return SimpleCompareResultEqual;
}

template <typename Type> ExtendedCompareResult extendedCompare(Type in_a, Type in_b)
{
	return (ExtendedCompareResult) simpleCompare(in_a, in_b);
}

#endif
