#ifndef _Interval_h
#define _Interval_h

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

template <typename Type> CompareResult compareIntervals(const Interval<Type>& in_a, const Interval<Type>& in_b)
{
	assert(in_a.const_x0()<=in_a.const_x1());
	assert(in_b.const_x1()<=in_b.const_x1());

	if (in_a.const_x1()<in_b.const_x0())
		return Less;
	if (in_a.const_x0()>in_b.const_x1())
		return Greater;
	if (in_a.const_x0()==in_b.const_x0() && in_a.const_x1()==in_b.const_x1())
		return Equal;
	if (in_a.const_x0()<=in_b.const_x0() && in_a.const_x1()>=in_b.const_x1())
		return Contains;
	if (in_a.const_x0()>=in_b.const_x0() && in_a.const_x1()<=in_b.const_x1())
		return ContainedIn;
	if (in_a.const_x0()<in_b.const_x0() && in_a.const_x1()<in_b.const_x1())
		return NotComparableLess;
	if (in_a.const_x0()>in_b.const_x0() && in_a.const_x1()>in_b.const_x1())
		return NotComparableGreater;

	return ErrorInCode;
}

#endif
