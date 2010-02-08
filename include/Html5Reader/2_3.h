#ifndef _2_3_h
#define _2_3_h

// 2.3 Case-sensitivity and string comparison
// Draft Standard — 6 February 2010

#include "BasicDataStructures/HigherOrderFunctions.h"
#include "BasicDataStructures/Unicode.h"

inline bool compareTokenCaseSensitive(UnicodeCodePoint p1, UnicodeCodePoint p2)
{
	return p1 == p2;
}

inline void convertTokenToASCIILowercase(UnicodeCodePoint* in_pp);

inline bool compareTokenASCIICaseInsensitive(UnicodeCodePoint p1, UnicodeCodePoint p2)
{
	convertTokenToASCIILowercase(&p1);
	convertTokenToASCIILowercase(&p2);
	return p1 == p2;
}

// not implemented yet
// inline bool compareTokenCompatibilityCaseless(UnicodeCodePoint p1, UnicodeCodePoint p2)

inline void convertTokenToASCIIUppercase(UnicodeCodePoint* in_pp)
{
	if (*in_pp>=0x0061 && *in_pp<=0x007A)
		*in_pp -= (0x0061-0x0041);
}

template <typename State> void convertStringToASCIIUppercase(
	Iterator<State, UnicodeCodePoint>* in_pIt)
{
	map(in_it, &convertTokenToASCIIUppercase);
}

inline void convertTokenToASCIILowercase(UnicodeCodePoint* in_pp)
{
	if (*in_pp>=0x0041 && *in_pp<=0x005A)
		*in_pp += (0x0061-0x0041);
}

template <typename State> void convertStringToASCIILowercase(
	Iterator<State, UnicodeCodePoint>* in_pIt)
{
	map(in_it, &convertTokenToASCIILowercase);
}

template <typename State> bool prefixMatch(
	Iterator<State, UnicodeCodePoint>* in_pPatternIt,
	Iterator<State, UnicodeCodePoint>* in_pStringIt)
{
	assert(in_pPatternIt->mfpEnd);
	assert(in_pPatternIt->mfpGet);
	assert(in_pPatternIt->mfpNext);

	assert(in_pStringIt->mfpEnd);
	assert(in_pStringIt->mfpGet);
	assert(in_pStringIt->mfpNext);

	while (true)
	{
		if ((*(in_pPatternIt->mfpEnd))())
			return true;

		if ((*(in_pStringIt->mfpEnd))())
			return false;

		if ((*(in_pPatternIt->mfpGet))()!=(*(in_pStringIt->mfpGet))())
			return false;

		(*(in_pPatternIt->mfpNext))(in_pPatternIt->mState);
		(*(in_pStringIt->mfpNext))(in_pStringIt->mState);
	}
}

#endif
