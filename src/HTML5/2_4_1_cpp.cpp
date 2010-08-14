#include "HTML5/2_4_1.h"
#include "Algorithm/BinarySearch.h"
#include "Unicode/PropList.h"
#include "Util/Interval.hpp"

int isWhiteSpaceCharacter(UnicodeCodePoint in_p, void* in_pWhitespaceIntervals, 
						   size_t in_whitespaceIntervalsCount)
{
	return binarySearch(&in_p, in_pWhitespaceIntervals, in_whitespaceIntervalsCount, 
		sizeof Interval<UnicodeCodePoint>, 
		&compareElementIntervalVoid<UnicodeCodePoint>) != -1;
}
