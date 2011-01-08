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

#ifndef _CompareResult_h
#define _CompareResult_h

typedef enum _SimpleCompareResult
{
	SimpleCompareResultEqual,         // the objects are identical
	SimpleCompareResultLess,
	SimpleCompareResultGreater
} SimpleCompareResult;


typedef enum _IntermediateCompareResult
{
	IntermediateCompareResultEqual,         // the objects are identical
	IntermediateCompareResultLess,
	IntermediateCompareResultGreater, 
	IntermediateCompareResultContainedIn,   // for example when comparing object with interval: the object is contained in 
	                                        // the interval, but the interval not only consists of a single point
} IntermediateCompareResult;


typedef enum _ExtendedCompareResult
{
	ExtendedCompareResultEqual,         // the objects are identical
	ExtendedCompareResultLess,
	ExtendedCompareResultGreater, 
	ExtendedCompareResultContainedIn,   // for example when comparing intervals: one interval is contained in the other
	ExtendedCompareResultContains,      // for example when comparing intervals: one interval contains the other
	
	// Now to 2 special cases
	// for example when comparing intervals: the intervals have a non-empty intersection
	// that is not identical to one of the intervals
	ExtendedCompareResultNotComparableLess,    // for example when the rest of the interval is less than their intersection
	ExtendedCompareResultNotComparableGreater, // for example when the rest of the interval is greater than their intersection

	ExtendedCompareResultErrorInCode           // this should never be returned (it is there to detect errors in the code as
	                      // forgotten cases)
} ExtendedCompareResult;

#endif
