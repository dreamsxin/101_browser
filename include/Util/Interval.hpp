#ifndef _Interval_hpp
#define _Interval_hpp

#include "BasicDataStructures/Comparator.h"
#include <cassert>

/*!
A closed interval.

x0 is the lower endpoint, x1 the higher endpoint.

We must have: x0<=x1
*/
template <typename T> class Interval
{
protected:
	T mX0, mX1;

public:
	Interval(T in_x0, T in_x1)
	{
		if (in_x0<=in_x1)
		{
			mX0=in_x0;
			mX1=in_x1;
		}
		else
		{
			mX0=in_x1;
			mX1=in_x0;
		}
	}

#if 0
	T& x0()
	{
		return mX0;
	}

	T& x1()
	{
		return mX1;
	}
#endif

	const T& const_x0() const
	{
		return mX0;
	}

	const T& const_x1() const
	{
		return mX1;
	}
};

template <typename Type> ExtendedCompareResult compareIntervals(const Interval<Type>& in_a, const Interval<Type>& in_b)
{
	assert(in_a.const_x0()<=in_a.const_x1());
	assert(in_b.const_x1()<=in_b.const_x1());

	if (in_a.const_x1()<in_b.const_x0())
		return ExtendedCompareResultLess;
	if (in_a.const_x0()>in_b.const_x1())
		return ExtendedCompareResultGreater;
	if (in_a.const_x0()==in_b.const_x0() && in_a.const_x1()==in_b.const_x1())
		return ExtendedCompareResultEqual;
	if (in_a.const_x0()<=in_b.const_x0() && in_a.const_x1()>=in_b.const_x1())
		return ExtendedCompareResultContains;
	if (in_a.const_x0()>=in_b.const_x0() && in_a.const_x1()<=in_b.const_x1())
		return ExtendedCompareResultContainedIn;
	if (in_a.const_x0()<in_b.const_x0() && in_a.const_x1()<in_b.const_x1())
		return ExtendedCompareResultNotComparableLess;
	if (in_a.const_x0()>in_b.const_x0() && in_a.const_x1()>in_b.const_x1())
		return ExtendedCompareResultNotComparableGreater;

	return ExtendedCompareResultErrorInCode;
}

#endif
