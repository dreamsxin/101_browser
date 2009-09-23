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
