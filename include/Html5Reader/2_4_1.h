#ifndef _2_4_1_h
#define _2_4_1_h

// 2.4.1 Common parser idioms
// Draft Standard — 6 February 2010

#include "BasicDataStructures/Unicode.h"
#include "BasicDataStructures/Collections.h"

inline bool isSpaceCharacter(UnicodeCodePoint in_p)
{
	switch (in_p)
	{
	case 0x0020: // SPACE, 
	case 0x0009: // CHARACTER TABULATION (tab)
	case 0x000A: // LINE FEED (LF)
	case 0x000C: // FORM FEED (FF)
	case 0x000D: // CARRIAGE RETURN (CR)
		return true;
	default:
		return false;
	}
}

// not implemented yet
// inline bool isWhiteSpaceCharacter(UnicodeCodePoint in_p)

inline bool isAlphanumericASCIICharacter(UnicodeCodePoint in_p)
{
	return (
		(0x0030 <= in_p && in_p <= 0x0039) //U+0030 DIGIT ZERO (0) to U+0039 DIGIT NINE (9) 
		||
		(0x0041 <= in_p && in_p <= 0x005A) // U+0041 LATIN CAPITAL LETTER A to U+005A LATIN CAPITAL LETTER Z
		|| 
		(0x0061 <= in_p && in_p <= 0x007A) // U+0061 LATIN SMALL LETTER A to U+007A LATIN SMALL LETTER Z
		);
}

inline bool collectSequenceOfCharacters()
{

}

#endif
