#ifndef _ParserUnicodeEndodings_h
#define _ParserUnicodeEndodings_h

#include "parser/ParserDataStructures.h"

struct Utf8Token
{
	unsigned char data[4]; // it is assumed that only as many bytes as necessary are filled
	                       // and only validly encoded characters get stored into this
	                       // data structure
};

/*!
 in_mask:   contains 1 at the positions which shall be compared
            contains 0 at the positions which shan't be compared
 in_value1: the 1st value to compare
 in_value2: the 2nd value to compare

 The functions checks whether in_value1 and in_value2 match in all
 bits of in_mask
 */
template <typename T> bool compareBitMask(T in_mask, T in_value1, T in_value2)
{
	return 
		!
		((in_mask & ( in_value1 ^  in_value2))   // compare the 1s
		||
		 (in_mask & (~in_value1 ^ ~in_value2))); // compare the 0s
}

Result<unsigned char> getNumberOfEncodingBytes(Utf8Token token);

#endif
