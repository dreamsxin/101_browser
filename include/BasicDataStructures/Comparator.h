#ifndef _Comparator_h
#define _Comparator_h

enum CompareResult
{
	Equal,         // the objects are identical
	Less,
	Greater, 
	Contains,      // for example when comparing intervals: one interval contains the other
	ContainedIn,   // for example when comparing intervals: one interval is contained in the other

	// Now to 2 special cases
	// for example when comparing intervals: the intervals have a non-empty intersection
	// that is not identical to one of the intervals
	NotComparableLess,    // for example when the rest of the interval is less than their intersection
	NotComparableGreater, // for example when the rest of the interval is greater than their intersection

	ErrorInCode           // this should never be returned (it is there to detect errors in the code as
	                      // forgotten cases)
};

template <typename Type> inline CompareResult simpleCompare(Type in_a, Type in_b)
{
	if (in_a<in_b)
		return Less;
	if (in_a>in_b)
		return Greater;

	return Equal;
}

#endif
