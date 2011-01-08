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

#ifndef _Permuter_h
#define _Permuter_h

#include <algorithm>

template <typename Type> void permuteForward(Type** in_pArray, size_t in_count)
{
	if (in_count==0)
		return;

	for (size_t idx=0; idx<in_count-1; idx++)
	{
		std::swap(*in_pArray[in_count-1-idx], *in_pArray[in_count-idx-2]);
	}
}

template <typename Type> void permuteBackward(Type** in_pArray, size_t in_count)
{
	if (in_count==0)
		return;

	for (size_t idx=0; idx<in_count-1; idx++)
	{
		std::swap(*in_pArray[idx], *in_pArray[idx+1]);
	}
}

#endif
