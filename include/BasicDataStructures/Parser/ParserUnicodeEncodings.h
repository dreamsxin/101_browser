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

#ifndef _ParserUnicodeEndodings_h
#define _ParserUnicodeEndodings_h

#include "BasicDataStructures/Parser/ParserDataStructures.h"

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
