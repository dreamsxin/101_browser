#include "HTML5/2_4_1.h"

bool allocWhitespaceIntervals(FILE* in_propListFile, void** in_pWhitespaceIntervals, 
                              size_t* in_pWhitespaceIntervalsCount)
{
	return readPropList(in_propListFile, "White_Space", in_pWhitespaceIntervals, in_pWhitespaceIntervalsCount);
}

void freeWhitespaceIntervals(void** in_pWhitespaceIntervals)
{
	freeIntervalsFromPropList(in_pWhitespaceIntervals);
}

