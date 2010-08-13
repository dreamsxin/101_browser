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
