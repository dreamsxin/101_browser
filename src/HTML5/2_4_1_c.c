#include "HTML5/2_4_1.h"
#include "Unicode/PropList.h"

bool allocWhite_SpaceCharacterIntervals(FILE* in_propListFile, void** in_pWhitespaceIntervals, 
                              size_t* in_pWhitespaceIntervalsCount)
{
	return readPropList(in_propListFile, "White_Space", in_pWhitespaceIntervals, in_pWhitespaceIntervalsCount);
}

void freeWhite_SpaceCharacterIntervals(void** in_pWhitespaceIntervals)
{
	freeIntervalsFromPropList(in_pWhitespaceIntervals);
}

