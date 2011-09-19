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
public:
	T mX0, mX1;

	const T& const_x0() const
	{
		return mX0;
	}

	const T& const_x1() const
	{
		return mX1;
	}
};

template <typename T> Interval<T> createInterval(T in_x0, T in_x1)
{
	T mX0, mX1;
	
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
	
	Interval<T> out_interval = { mX0, mX1 };
	return out_interval;
}

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

	assert(false);
}

template <typename Type> IntermediateCompareResult compareElementInterval
(const Type* in_pElement, const Interval<Type>* in_pInterval)
{
	assert(in_pInterval->const_x0() <= in_pInterval->const_x1());

	if (*in_pElement < in_pInterval->const_x0())
		return IntermediateCompareResultLess;
	else if (*in_pElement > in_pInterval->const_x1())
		return IntermediateCompareResultGreater;
	else if (in_pInterval->const_x0() == in_pInterval->const_x1())
	{
		assert(*in_pElement == in_pInterval->const_x0());
		assert(*in_pElement == in_pInterval->const_x1());

		return IntermediateCompareResultEqual;
	}
	else
	{
		assert(*in_pElement >= in_pInterval->const_x0());
		assert(*in_pElement <= in_pInterval->const_x1());

		return IntermediateCompareResultContainedIn;
	}
}

template <typename Type> IntermediateCompareResult compareElementIntervalVoid
(const void* in_pElement, const void* in_pInterval)
{
	return compareElementInterval(static_cast<const Type*>(in_pElement), static_cast<const Interval<Type>*>(in_pInterval));
}

#endif
