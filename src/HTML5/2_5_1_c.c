#include <assert.h>
#include "HTML5/2_5_1.h"
#include "Algorithm/IterateWhile.h"
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

bool skipWhitespaceKernel(const void* in_pUnicodeCodePoint, const void* in_pUserdata)
{
	UnicodeCodePoint *pUnicodeCodePoint = (UnicodeCodePoint*) in_pUnicodeCodePoint;

	assert(pUnicodeCodePoint != NULL);

	if (pUnicodeCodePoint != NULL)
		return isSpaceCharacter(*pUnicodeCodePoint);
	else
		return false;
}

bool skipWhitespace(SingleIterator in_iterator, void* in_iteratorState)
{
	return iterateWhile(in_iterator, in_iteratorState, NULL, &skipWhitespaceKernel);
}

struct SkipWhite_SpaceCharactersUserdata
{
	 void* mpWhitespaceIntervals;
	 size_t mWhitespaceIntervalsCount;
};

bool skipWhite_SpaceCharactersKernel(const void* in_pUnicodeCodePoint, const void* in_pUserdata)
{
	UnicodeCodePoint *pUnicodeCodePoint = (UnicodeCodePoint*) in_pUnicodeCodePoint;
	struct SkipWhite_SpaceCharactersUserdata *pUserData = 
		(struct SkipWhite_SpaceCharactersUserdata*) in_pUserdata;

	assert(pUnicodeCodePoint != NULL);
	assert(pUserData != NULL);

	if (pUnicodeCodePoint != NULL && pUserData != NULL)
		return isWhite_SpaceCharacter(*pUnicodeCodePoint, 
		pUserData->mpWhitespaceIntervals, pUserData->mWhitespaceIntervalsCount);
	else
		return false;
}

bool skipWhite_SpaceCharacters(SingleIterator in_iterator, void* in_iteratorState, 
							   void* in_pWhitespaceIntervals, size_t in_whitespaceIntervalsCount)
{
	struct SkipWhite_SpaceCharactersUserdata userdata;
	userdata.mpWhitespaceIntervals = in_pWhitespaceIntervals;
	userdata.mWhitespaceIntervalsCount = in_whitespaceIntervalsCount;

	return iterateWhile(in_iterator, in_iteratorState, &userdata, &skipWhite_SpaceCharactersKernel);
}
