#ifndef _CompareResult_h
#define _CompareResult_h

typedef enum _SimpleCompareResult
{
	SimpleCompareResultEqual,         // the objects are identical
	SimpleCompareResultLess,
	SimpleCompareResultGreater
} SimpleCompareResult;

typedef enum _ExtendedCompareResult
{
	ExtendedCompareResultEqual,         // the objects are identical
	ExtendedCompareResultLess,
	ExtendedCompareResultGreater, 
	ExtendedCompareResultContains,      // for example when comparing intervals: one interval contains the other
	ExtendedCompareResultContainedIn,   // for example when comparing intervals: one interval is contained in the other

	// Now to 2 special cases
	// for example when comparing intervals: the intervals have a non-empty intersection
	// that is not identical to one of the intervals
	ExtendedCompareResultNotComparableLess,    // for example when the rest of the interval is less than their intersection
	ExtendedCompareResultNotComparableGreater, // for example when the rest of the interval is greater than their intersection

	ExtendedCompareResultErrorInCode           // this should never be returned (it is there to detect errors in the code as
	                      // forgotten cases)
} ExtendedCompareResult;

#endif
