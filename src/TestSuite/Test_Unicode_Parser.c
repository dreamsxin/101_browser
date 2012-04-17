/*
* Copyright 2008-2012 Wolfgang Keller
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

#include "IO/MemoryByteStream_v4.h"
#include "Unicode/Parser_UTF8.h"
#include "Unicode/Parser_UTF16.h"
#include "Unicode/Parser_UTF32.h"
#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include <assert.h>
#include <stdio.h>
#include <string.h> // memcmp
#include <stdlib.h>

void test_Unicode_Parser_UTF8()
{
	UnicodeCodePoint bufferOut[1024];
	MemoryByteStream_v4State readState;
	MemoryByteStream_v4State writeState;
	UTF8_State utf8State;
	uint8_t triggerAsEarlyAsPossible;

	UnicodeCodePoint singleCodePoint;
	uint8_t singleByte;

	size_t idx;

	uint8_t bufferIn[] = {
		// taken from
		// http://www.whatwg.org/specs/web-apps/current-work/#utf-8
		// (24th of January 2011)
		0x41,             // 'A'
		0x98, 
		0xBA, 
		0x42,             // 'B'
		0xE2, 0x98, 
		0x43,             // 'C'
		0xE2, 0x98, 0xBA, // 0x263A
		0xE2, 0x98, 

		// taken from
		// http://en.wikipedia.org/w/index.php?title=UTF-8&oldid=485679924#Examples
		0x24, 
		0xC2, 0xA2, 
		0xE2, 0x82, 0xAC, 
		0xF0, 0xA4, 0xAD, 0xA2, 

		// taken from
		// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
		// (also in testfiles/Unicode/UTF-8-test.txt)

		// 2  Boundary condition test cases

		// 2.1  First possible sequence of a certain length

		// 2.1.1  1 byte  (U-00000000):
		0x00, 
		// 2.1.2  2 bytes (U-00000080):
		0xC2, 0x80, 
		// 2.1.3  3 bytes (U-00000800):
		0xE0, 0xA0, 0x80, 
		// 2.1.4  4 bytes (U-00010000):
		0xF0, 0x90, 0x80, 0x80, 
		// 2.1.5  5 bytes (U-00200000):
		0xF8, 0x88, 0x80, 0x80, 0x80, 
		// 2.1.6  6 bytes (U-04000000):
		0xFC, 0x84, 0x80, 0x80, 0x80, 0x80, 

		// 2.2  Last possible sequence of a certain length

		// 2.2.1  1 byte  (U-0000007F):
		0x7F, 
		// 2.2.2  2 bytes (U-000007FF):
		0xDF, 0xBF, 
		// 2.2.3  3 bytes (U-0000FFFF):
		0xEF, 0xBF, 0xBF, 
		// 2.2.4  4 bytes (U-001FFFFF):
		0xF7, 0xBF, 0xBF, 0xBF, 
		// 2.2.5  5 bytes (U-03FFFFFF):
		0xFB, 0xBF, 0xBF, 0xBF, 0xBF, 
		// 2.2.6  6 bytes (U-7FFFFFFF):
		0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 0xBF, 

		// 2.3  Other boundary conditions

		// 2.3.1  U-0000D7FF = ed 9f bf
		0xED, 0x9F, 0xBF, 
		// 2.3.2  U-0000E000 = ee 80 80
		0xEE, 0x80, 0x80, 
		// 2.3.3  U-0000FFFD = ef bf bd
		0xEF, 0xBF, 0xBD, 
		// 2.3.4  U-0010FFFF = f4 8f bf bf
		0xF4, 0x8F, 0xBF, 0xBF, 
		// 2.3.5  U-00110000 = f4 90 80 80
		0xF4, 0x90, 0x80, 0x80, 

		// 3  Malformed sequences

		// 3.1  Unexpected continuation bytes

		// 3.1.1  First continuation byte 0x80:
		0x80, 
		// 3.1.2  Last  continuation byte 0xbf:
		0xBF, 
		// 3.1.3  2 continuation bytes:
		0x80, 0xBF, 
		// 3.1.4  3 continuation bytes:
		0x80, 0xBF, 0x80, 
		// 3.1.5  4 continuation bytes:
		0x80, 0xBF, 0x80, 0xBF, 
		// 3.1.6  5 continuation bytes:
		0x80, 0xBF, 0x80, 0xBF, 0x80, 
		// 3.1.7  6 continuation bytes:
		0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF, 
		// 3.1.8  7 continuation bytes:
		0x80, 0xBF, 0x80, 0xBF, 0x80, 0xBF, 0x80, 
		// 3.1.9  Sequence of all 64 possible continuation bytes (0x80-0xbf):
		0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 
		0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F, 
		0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 
		0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 

		// 3.2  Lonely start characters

		// 3.2.1  All 32 first bytes of 2-byte sequences (0xc0-0xdf),
		//        each followed by a space character:
		0xC0, 0x20, 0xC1, 0x20, 0xC2, 0x20, 0xC3, 0x20, 0xC4, 0x20, 0xC5, 0x20, 0xC6, 0x20, 0xC7, 0x20, 
		0xC8, 0x20, 0xC9, 0x20, 0xCA, 0x20, 0xCB, 0x20, 0xCC, 0x20, 0xCD, 0x20, 0xCE, 0x20, 0xCF, 0x20, 
		0xD0, 0x20, 0xD1, 0x20, 0xD2, 0x20, 0xD3, 0x20, 0xD4, 0x20, 0xD5, 0x20, 0xD6, 0x20, 0xD7, 0x20, 
		0xD8, 0x20, 0xD9, 0x20, 0xDA, 0x20, 0xDB, 0x20, 0xDC, 0x20, 0xDD, 0x20, 0xDE, 0x20, 0xDF, 0x20, 
		// 3.2.2  All 16 first bytes of 3-byte sequences (0xe0-0xef),
		//        each followed by a space character:
		0xE0, 0x20, 0xE1, 0x20, 0xE2, 0x20, 0xE3, 0x20, 0xE4, 0x20, 0xE5, 0x20, 0xE6, 0x20, 0xE7, 0x20, 
		0xE8, 0x20, 0xE9, 0x20, 0xEA, 0x20, 0xEB, 0x20, 0xEC, 0x20, 0xED, 0x20, 0xEE, 0x20, 0xEF, 0x20, 
		// 3.2.3  All 8 first bytes of 4-byte sequences (0xf0-0xf7),
		//        each followed by a space character:
		0xF0, 0x20, 0xF1, 0x20, 0xF2, 0x20, 0xF3, 0x20, 0xF4, 0x20, 0xF5, 0x20, 0xF6, 0x20, 0xF7, 0x20, 
		// 3.2.4  All 4 first bytes of 5-byte sequences (0xf8-0xfb),
		//        each followed by a space character:
		0xF8, 0x20, 0xF9, 0x20, 0xFA, 0x20, 0xFB, 0x20, 
		// 3.2.5  All 2 first bytes of 6-byte sequences (0xfc-0xfd),
		//        each followed by a space character:
		0xFC, 0x20, 0xFD, 0x20, 

		// 3.3  Sequences with last continuation byte missing

		// 3.3.1  2-byte sequence with last byte missing (U+0000):
		0xC0, 
		// 3.3.2  3-byte sequence with last byte missing (U+0000):
		0xE0, 0x80, 
		// 3.3.3  4-byte sequence with last byte missing (U+0000):
		0xF0, 0x80, 0x80, 
		// 3.3.4  5-byte sequence with last byte missing (U+0000):
		0xF8, 0x80, 0x80, 0x80, 
		// 3.3.5  6-byte sequence with last byte missing (U+0000):
		0xFC, 0x80, 0x80, 0x80, 0x80, 
		// 3.3.6  2-byte sequence with last byte missing (U-000007FF):
		0xDF, 
		// 3.3.7  3-byte sequence with last byte missing (U-0000FFFF):
		0xEF, 0xBF, 
		// 3.3.8  4-byte sequence with last byte missing (U-001FFFFF):
		0xF7, 0xBF, 0xBF, 
		// 3.3.9  5-byte sequence with last byte missing (U-03FFFFFF):
		0xFB, 0xBF, 0xBF, 0xBF, 
		// 3.3.10 6-byte sequence with last byte missing (U-7FFFFFFF):
		0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 

		// 3.4  Concatenation of incomplete sequences
		// All the 10 sequences of 3.3 concatenated, you should see 10 malformed
		// sequences being signalled:
		0xC0, 
		0xE0, 0x80, 
		0xF0, 0x80, 0x80, 
		0xF8, 0x80, 0x80, 0x80, 
		0xFC, 0x80, 0x80, 0x80, 0x80, 
		0xDF, 
		0xEF, 0xBF, 
		0xF7, 0xBF, 0xBF, 
		0xFB, 0xBF, 0xBF, 0xBF, 
		0xFD, 0xBF, 0xBF, 0xBF, 0xBF, 

		// 3.5  Impossible bytes

		// 3.5.1  fe
		0xFE, 
		// 3.5.2  ff
		0xFF, 
		// 3.5.3  fe fe ff ff
		0xFE, 0xFE, 0xFF, 0xFF, 

		// 4  Overlong sequences

		// 4.1  Examples of an overlong ASCII character

		// 4.1.1 U+002F = c0 af
		0xC0, 0xAF, 
		// 4.1.2 U+002F = e0 80 af
		0xE0, 0x80, 0xAF, 
		// 4.1.3 U+002F = f0 80 80 af
		0xF0, 0x80, 0x80, 0xAF, 
		// 4.1.4 U+002F = f8 80 80 80 af
		0xF8, 0x80, 0x80, 0x80, 0xAF, 
		// 4.1.5 U+002F = fc 80 80 80 80 af
		0xFC, 0x80, 0x80, 0x80, 0x80, 0xAF, 

		// 4.2  Maximum overlong sequences

		// 4.2.1  U-0000007F = c1 bf
		0xC1, 0xBF, 
		// 4.2.2  U-000007FF = e0 9f bf
		0xE0, 0x9F, 0xBF, 
		// 4.2.3  U-0000FFFF = f0 8f bf bf
		0xF0, 0x8F, 0xBF, 0xBF, 
		// 4.2.4  U-001FFFFF = f8 87 bf bf bf
		0xF8, 0x87, 0xBF, 0xBF, 0xBF, 
		// 4.2.5  U-03FFFFFF = fc 83 bf bf bf bf
		0xFC, 0x83, 0xBF, 0xBF, 0xBF, 0xBF, 

		// 4.3  Overlong representation of the NUL character

		// 4.3.1  U+0000 = c0 80
		0xC0, 0x80, 
		// 4.3.2  U+0000 = e0 80 80
		0xE0, 0x80, 0x80, 
		// 4.3.3  U+0000 = f0 80 80 80
		0xF0, 0x80, 0x80, 0x80, 
		// 4.3.4  U+0000 = f8 80 80 80 80
		0xF8, 0x80, 0x80, 0x80, 0x80, 
		// 4.3.5  U+0000 = fc 80 80 80 80 80
		0xFC, 0x80, 0x80, 0x80, 0x80, 0x80, 

		// 5  Illegal code positions

		// 5.1 Single UTF-16 surrogates

		// 5.1.1  U+D800 = ed a0 80
		0xED, 0xA0, 0x80, 
		// 5.1.2  U+DB7F = ed ad bf
		0xED, 0xAD, 0xBF, 
		// 5.1.3  U+DB80 = ed ae 80
		0xED, 0xAE, 0x80, 
		// 5.1.4  U+DBFF = ed af bf
		0xED, 0xAF, 0xBF, 
		// 5.1.5  U+DC00 = ed b0 80
		0xED, 0xB0, 0x80, 
		// 5.1.6  U+DF80 = ed be 80
		0xED, 0xBE, 0x80, 
		// 5.1.7  U+DFFF = ed bf bf
		0xED, 0xBF, 0xBF, 

		// 5.2 Paired UTF-16 surrogates

		// 5.2.1  U+D800 U+DC00 = ed a0 80 ed b0 80
		0xED, 0xA0, 0x80, 0xED, 0xB0, 0x80, 
		// 5.2.2  U+D800 U+DFFF = ed a0 80 ed bf bf
		0xED, 0xA0, 0x80, 0xED, 0xBF, 0xBF, 
		// 5.2.3  U+DB7F U+DC00 = ed ad bf ed b0 80
		0xED, 0xAD, 0xBF, 0xED, 0xB0, 0x80, 
		// 5.2.4  U+DB7F U+DFFF = ed ad bf ed bf bf
		0xED, 0xAD, 0xBF, 0xED, 0xBF, 0xBF, 
		// 5.2.5  U+DB80 U+DC00 = ed ae 80 ed b0 80
		0xED, 0xAE, 0x80, 0xED, 0xB0, 0x80, 
		// 5.2.6  U+DB80 U+DFFF = ed ae 80 ed bf bf
		0xED, 0xAE, 0x80, 0xED, 0xBF, 0xBF, 
		// 5.2.7  U+DBFF U+DC00 = ed af bf ed b0 80
		0xED, 0xAF, 0xBF, 0xED, 0xB0, 0x80, 
		// 5.2.8  U+DBFF U+DFFF = ed af bf ed bf bf
		0xED, 0xAF, 0xBF, 0xED, 0xBF, 0xBF

		// 5.3 Other illegal code positions

		// 5.3.1  U+FFFE = ef bf be
		// 5.3.2  U+FFFF = ef bf bf
	};
	UnicodeCodePoint result[] = {
		// taken from
		// http://www.whatwg.org/specs/web-apps/current-work/#utf-8
		// (24th of January 2011)
		'A', 
		0xFFFD, 
		0xFFFD, 
		'B', 
		0xFFFD, 
		'C', 
		0x263A, 
		0xFFFD, 

		// taken from
		// http://en.wikipedia.org/w/index.php?title=UTF-8&oldid=485679924#Examples
		0x0024, 
		0x00A2, 
		0x20AC, 
		0x24B62, 

		// taken from
		// http://www.cl.cam.ac.uk/~mgk25/ucs/examples/UTF-8-test.txt
		// (also in testfiles/Unicode/UTF-8-test.txt)

		// 2  Boundary condition test cases

		// 2.1  First possible sequence of a certain length

		// 2.1.1  1 byte  (U-00000000):
		0x00, 
		// 2.1.2  2 bytes (U-00000080):
		0x80, 
		// 2.1.3  3 bytes (U-00000800):
		0x800, 
		// 2.1.4  4 bytes (U-00010000):
		0x10000, 
		// 2.1.5  5 bytes (U-00200000):
		0xFFFD, 
		// 2.1.6  6 bytes (U-04000000):
		0xFFFD, 

		// 2.2  Last possible sequence of a certain length

		// 2.2.1  1 byte  (U-0000007F):
		0x7F, 
		// 2.2.2  2 bytes (U-000007FF):
		0x7FF, 
		// 2.2.3  3 bytes (U-0000FFFF):
		0xFFFF, 
		// 2.2.4  4 bytes (U-001FFFFF):
		0xFFFD, 
		// 2.2.5  5 bytes (U-03FFFFFF):
		0xFFFD, 
		// 2.2.6  6 bytes (U-7FFFFFFF):
		0xFFFD, 

		// 2.3  Other boundary conditions

		// 2.3.1  U-0000D7FF = ed 9f bf
		0xD7FF, 
		// 2.3.2  U-0000E000 = ee 80 80
		0xE000, 
		// 2.3.3  U-0000FFFD = ef bf bd
		0xFFFD, 
		// 2.3.4  U-0010FFFF = f4 8f bf bf
		0x10FFFF, 
		// 2.3.5  U-00110000 = f4 90 80 80
		0xFFFD, 

		// 3  Malformed sequences

		// 3.1  Unexpected continuation bytes

		// 3.1.1  First continuation byte 0x80:
		0xFFFD, 
		// 3.1.2  Last  continuation byte 0xbf:
		0xFFFD, 
		// 3.1.3  2 continuation bytes:
		0xFFFD, 0xFFFD, 
		// 3.1.4  3 continuation bytes:
		0xFFFD, 0xFFFD, 0xFFFD, 
		// 3.1.5  4 continuation bytes:
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		// 3.1.6  5 continuation bytes:
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		// 3.1.7  6 continuation bytes:
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		// 3.1.8  7 continuation bytes:
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		// 3.1.9  Sequence of all 64 possible continuation bytes (0x80-0xbf):
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 

		// 3.2  Lonely start characters

		// 3.2.1  All 32 first bytes of 2-byte sequences (0xc0-0xdf),
		//        each followed by a space character:
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		// 3.2.2  All 16 first bytes of 3-byte sequences (0xe0-0xef),
		//        each followed by a space character:
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		// 3.2.3  All 8 first bytes of 4-byte sequences (0xf0-0xf7),
		//        each followed by a space character:
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		// 3.2.4  All 4 first bytes of 5-byte sequences (0xf8-0xfb),
		//        each followed by a space character:
		0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 0xFFFD, 0x20, 
		// 3.2.5  All 2 first bytes of 6-byte sequences (0xfc-0xfd),
		//        each followed by a space character:
		0xFFFD, 0x20, 0xFFFD, 0x20, 

		// 3.3  Sequences with last continuation byte missing

		// 3.3.1  2-byte sequence with last byte missing (U+0000):
		0xFFFD, 
		// 3.3.2  3-byte sequence with last byte missing (U+0000):
		0xFFFD, 
		// 3.3.3  4-byte sequence with last byte missing (U+0000):
		0xFFFD, 
		// 3.3.4  5-byte sequence with last byte missing (U+0000):
		0xFFFD, 
		// 3.3.5  6-byte sequence with last byte missing (U+0000):
		0xFFFD, 
		// 3.3.6  2-byte sequence with last byte missing (U-000007FF):
		0xFFFD, 
		// 3.3.7  3-byte sequence with last byte missing (U-0000FFFF):
		0xFFFD, 
		// 3.3.8  4-byte sequence with last byte missing (U-001FFFFF):
		0xFFFD, 
		// 3.3.9  5-byte sequence with last byte missing (U-03FFFFFF):
		0xFFFD, 
		// 3.3.10 6-byte sequence with last byte missing (U-7FFFFFFF):
		0xFFFD, 

		// 3.4  Concatenation of incomplete sequences
		// All the 10 sequences of 3.3 concatenated, you should see 10 malformed
		// sequences being signalled:
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 

		// 3.5  Impossible bytes

		// 3.5.1  fe
		0xFFFD, 
		// 3.5.2  ff
		0xFFFD, 
		// 3.5.3  fe fe ff ff
		0xFFFD, 0xFFFD, 0xFFFD, 0xFFFD, 


		// 4  Overlong sequences

		// 4.1  Examples of an overlong ASCII character

		// 4.1.1 U+002F = c0 af
		0xFFFD, 
		// 4.1.2 U+002F = e0 80 af
		0xFFFD, 
		// 4.1.3 U+002F = f0 80 80 af
		0xFFFD, 
		// 4.1.4 U+002F = f8 80 80 80 af
		0xFFFD, 
		// 4.1.5 U+002F = fc 80 80 80 80 af
		0xFFFD, 

		// 4.2  Maximum overlong sequences

		// 4.2.1  U-0000007F = c1 bf
		0xFFFD, 
		// 4.2.2  U-000007FF = e0 9f bf
		0xFFFD, 
		// 4.2.3  U-0000FFFF = f0 8f bf bf
		0xFFFD, 
		// 4.2.4  U-001FFFFF = f8 87 bf bf bf
		0xFFFD, 
		// 4.2.5  U-03FFFFFF = fc 83 bf bf bf bf
		0xFFFD, 

		// 4.3  Overlong representation of the NUL character

		// 4.3.1  U+0000 = c0 80
		0xFFFD, 
		// 4.3.2  U+0000 = e0 80 80
		0xFFFD, 
		// 4.3.3  U+0000 = f0 80 80 80
		0xFFFD, 
		// 4.3.4  U+0000 = f8 80 80 80 80
		0xFFFD, 
		// 4.3.5  U+0000 = fc 80 80 80 80 80
		0xFFFD, 

		// 5  Illegal code positions

		// 5.1 Single UTF-16 surrogates

		// 5.1.1  U+D800 = ed a0 80
		0xFFFD, 
		// 5.1.2  U+DB7F = ed ad bf
		0xFFFD, 
		// 5.1.3  U+DB80 = ed ae 80
		0xFFFD, 
		// 5.1.4  U+DBFF = ed af bf
		0xFFFD, 
		// 5.1.5  U+DC00 = ed b0 80
		0xFFFD, 
		// 5.1.6  U+DF80 = ed be 80
		0xFFFD, 
		// 5.1.7  U+DFFF = ed bf bf
		0xFFFD, 

		// 5.2 Paired UTF-16 surrogates

		// 5.2.1  U+D800 U+DC00 = ed a0 80 ed b0 80
		0xFFFD, 0xFFFD, 
		// 5.2.2  U+D800 U+DFFF = ed a0 80 ed bf bf
		0xFFFD, 0xFFFD, 
		// 5.2.3  U+DB7F U+DC00 = ed ad bf ed b0 80
		0xFFFD, 0xFFFD, 
		// 5.2.4  U+DB7F U+DFFF = ed ad bf ed bf bf
		0xFFFD, 0xFFFD, 
		// 5.2.5  U+DB80 U+DC00 = ed ae 80 ed b0 80
		0xFFFD, 0xFFFD, 
		// 5.2.6  U+DB80 U+DFFF = ed ae 80 ed bf bf
		0xFFFD, 0xFFFD, 
		// 5.2.7  U+DBFF U+DC00 = ed af bf ed b0 80
		0xFFFD, 0xFFFD, 
		// 5.2.8  U+DBFF U+DFFF = ed af bf ed bf bf
		0xFFFD, 0xFFFD

		// 5.3 Other illegal code positions

		// 5.3.1  U+FFFE = ef bf be
		// 5.3.2  U+FFFF = ef bf bf
	};

	ByteStreamReadInterface_v4 readInterface = memoryByteStreamReadInterface_v4_get();
	ByteStreamWriteInterface_v4 writeInterface = memoryByteStreamWriteInterface_v4_get();

	ParseBlocker parseBlocker;
	size_t allReadCount;

	for (triggerAsEarlyAsPossible = 0; triggerAsEarlyAsPossible < 2; triggerAsEarlyAsPossible++)
	{
		// 1st test run

		memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
			(bool) triggerAsEarlyAsPossible, true);
		memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

		utf8_StateInit(&utf8State);

		allReadCount = 0;

		while ((parseBlocker = utf8_parse(&utf8State, &readState, readInterface, 
			&writeState, writeInterface)) != ParseBlocker_Neither)
		{
			test(ParseBlocker_Writer == parseBlocker);
			memoryByteStream_v4StateReset(&writeState);

			if (allReadCount >= sizeof(result) / sizeof(UnicodeCodePoint))
			{
				fprintf(stderr, "Buffer overflow detected in line %u of file %s. Exiting.", __LINE__, __FILE__);
				exit(EXIT_FAILURE);
			}

			test(result[allReadCount] == singleCodePoint);

			allReadCount++;
		}

		test(1 == writeState.currentBufferBlockIndex);
		allReadCount++;

		test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
		test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
			commonByteStreamInterface.mpfGetStatus(&readState));
		test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
			commonByteStreamInterface.mpfGetStatus(&writeState));

		test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

		// 2nd test run

		memoryByteStream_v4ReadStateInit(&readState, &singleByte, 1, 1, 
			(bool) triggerAsEarlyAsPossible, false);
		memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
			sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
		utf8_StateReset(&utf8State);

		for (idx = 0; idx < sizeof(bufferIn); idx++)
		{
			singleByte = bufferIn[idx];

			parseBlocker = utf8_parse(&utf8State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Reader == parseBlocker);
			test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
				writeState.blockSize * writeState.currentBufferBlockIndex));

			memoryByteStream_v4StateReset(&readState);
		}

		readInterface.commonByteStreamInterface.mpfSetStatus
			(&readState, ByteStreamStatus_Terminated);

		parseBlocker = utf8_parse(&utf8State, &readState, readInterface, 
			&writeState, writeInterface);

		test(ParseBlocker_Neither == parseBlocker);

		test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
			writeState.blockSize * writeState.currentBufferBlockIndex));

		test(writeState.blockSize * writeState.currentBufferBlockIndex == 
			sizeof(result));

		// 3rd test run

		memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
			(bool) triggerAsEarlyAsPossible, true);
		memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
			sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
		utf8_StateReset(&utf8State);

		parseBlocker = utf8_parse(&utf8State, &readState, readInterface, 
			&writeState, writeInterface);

		test(ParseBlocker_Neither == parseBlocker);

		test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
		test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
			commonByteStreamInterface.mpfGetStatus(&readState));
		test(ByteStreamStatus_Terminated == 
			writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
		test(!memcmp(result, bufferOut, sizeof(result)));

		// Testing behaviour after termination
		test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
		parseBlocker = utf8_parse(&utf8State, &readState, readInterface, 
			&writeState, writeInterface);
		test(ParseBlocker_Neither == parseBlocker);
		test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
	}
}

void test_Unicode_Parser_UTF16()
{
	UnicodeCodePoint bufferOut[32];
	MemoryByteStream_v4State readState;
	MemoryByteStream_v4State writeState;
	UTF16_State utf16State;
	uint8_t triggerAsEarlyAsPossible;

	UnicodeCodePoint singleCodePoint;
	uint8_t singleByte;

	size_t idx;

	const UnicodeCodePoint result[] = {
		0x00FF, 0x2020, 0xD7FF, 

		// Replacement character because of single High Surrogate
		0xFFFD, 

		// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
		0x007A, 0x6C34, 0x10000, 0x1D11E, 0x10FFFD, 

		// Replacement character because of single Low Surrogate
		0xFFFD
	};

	ParseBlocker parseBlocker;
	size_t allReadCount;

	ByteStreamReadInterface_v4 readInterface = memoryByteStreamReadInterface_v4_get();
	ByteStreamWriteInterface_v4 writeInterface = memoryByteStreamWriteInterface_v4_get();

	for (triggerAsEarlyAsPossible = 0; triggerAsEarlyAsPossible < 2; triggerAsEarlyAsPossible++)
	{
		// Big Endian
		{
			const uint8_t bufferIn[] = {
				0x00, 0xFF, 
				0x20, 0x20, 
				0xD7, 0xFF, 

				// Single High Surrogate
				0xD8, 0x00, 

				// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
				0x00, 0x7A, 
				0x6C, 0x34, 
				0xD8, 0x00, 0xDC, 0x00, 
				0xD8, 0x34, 0xDD, 0x1E, 
				0xDB, 0xFF, 0xDF, 0xFD, 

				// Single Low Surrogate
				0xDC, 0x00
			};

			// 1st test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf16_StateInit(&utf16State, true);

			allReadCount = 0;

			while ((parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				if (allReadCount >= sizeof(result) / sizeof(UnicodeCodePoint))
				{
					fprintf(stderr, "Buffer overflow detected in line %u of file %s. Exiting.", __LINE__, __FILE__);
					exit(EXIT_FAILURE);
				}

				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			// 2nd test run

			memoryByteStream_v4ReadStateInit(&readState, &singleByte, 1, 1, 
				(bool) triggerAsEarlyAsPossible, false);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf16_StateReset(&utf16State);

			for (idx = 0; idx < sizeof(bufferIn); idx++)
			{
				singleByte = bufferIn[idx];

				parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
					&writeState, writeInterface);

				test(ParseBlocker_Reader == parseBlocker);
				test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
					writeState.blockSize * writeState.currentBufferBlockIndex));

				memoryByteStream_v4StateReset(&readState);
			}

			readInterface.commonByteStreamInterface.mpfSetStatus
				(&readState, ByteStreamStatus_Terminated);

			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
				writeState.blockSize * writeState.currentBufferBlockIndex));

			test(writeState.blockSize * writeState.currentBufferBlockIndex == 
				sizeof(result));

			// 3rd test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf16_StateReset(&utf16State);

			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
			test(!memcmp(result, bufferOut, sizeof(result)));

			// Testing behaviour after termination
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);
			test(ParseBlocker_Neither == parseBlocker);
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
		}

		// Little Endian
		{
			const uint8_t bufferIn[] = {
				0xFF, 0x00, 
				0x20, 0x20, 
				0xFF, 0xD7, 

				// Single High Surrogate
				0x00, 0xD8, 

				// Taken from http://en.wikipedia.org/w/index.php?title=UTF-16&oldid=474632903#Examples
				0x7A, 0x00, 
				0x34, 0x6C, 
				0x00, 0xD8, 0x00, 0xDC, 
				0x34, 0xD8, 0x1E, 0xDD, 
				0xFF, 0xDB, 0xFD, 0xDF, 

				// Single Low Surrogate
				0x00, 0xDC
			};

			// 1st test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf16_StateInit(&utf16State, false);

			allReadCount = 0;

			while ((parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				if (allReadCount >= sizeof(result) / sizeof(UnicodeCodePoint))
				{
					fprintf(stderr, "Buffer overflow detected in line %u of file %s. Exiting.", __LINE__, __FILE__);
					exit(EXIT_FAILURE);
				}

				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));

			// 2nd test run

			memoryByteStream_v4ReadStateInit(&readState, &singleByte, 1, 1, 
				(bool) triggerAsEarlyAsPossible, false);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf16_StateReset(&utf16State);

			for (idx = 0; idx < sizeof(bufferIn); idx++)
			{
				singleByte = bufferIn[idx];

				parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
					&writeState, writeInterface);

				test(ParseBlocker_Reader == parseBlocker);
				test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
					writeState.blockSize * writeState.currentBufferBlockIndex));

				memoryByteStream_v4StateReset(&readState);
			}

			readInterface.commonByteStreamInterface.mpfSetStatus
				(&readState, ByteStreamStatus_Terminated);

			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
				writeState.blockSize * writeState.currentBufferBlockIndex));

			test(writeState.blockSize * writeState.currentBufferBlockIndex == 
				sizeof(result));

			// 3rd test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf16_StateReset(&utf16State);

			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
			test(!memcmp(result, bufferOut, sizeof(result)));

			// Testing behaviour after termination
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
			parseBlocker = utf16_parse(&utf16State, &readState, readInterface, 
				&writeState, writeInterface);
			test(ParseBlocker_Neither == parseBlocker);
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
		}
	}
}

void test_Unicode_Parser_UTF32()
{
	UnicodeCodePoint bufferOut[32];
	MemoryByteStream_v4State readState;
	MemoryByteStream_v4State writeState;
	UTF32_State utf32State;
	uint8_t triggerAsEarlyAsPossible;

	UnicodeCodePoint singleCodePoint;
	uint8_t singleByte;

	size_t idx;

	const UnicodeCodePoint result[] = {
		0x00, 
		0xD7FF, 
		0xFFFD, // replacement character
		0xFFFD, // replacement character
		0xE000, 
		0x10FFFF, 
		0xFFFD, // replacement character
		0xFFFD, // replacement character
		0xFFFD  // replacement character
	};

	ParseBlocker parseBlocker;
	size_t allReadCount;

	ByteStreamReadInterface_v4 readInterface = memoryByteStreamReadInterface_v4_get();
	ByteStreamWriteInterface_v4 writeInterface = memoryByteStreamWriteInterface_v4_get();

	for (triggerAsEarlyAsPossible = 0; triggerAsEarlyAsPossible < 2; triggerAsEarlyAsPossible++)
	{
		// Big Endian
		{
			const uint8_t bufferIn[] = {
				0x00, 0x00, 0x00, 0x00, 
				0x00, 0x00, 0xD7, 0xFF, 
				0x00, 0x00, 0xD8, 0x00, 
				0x00, 0x00, 0xDF, 0xFF, 
				0x00, 0x00, 0xE0, 0x00, 
				0x00, 0x10, 0xFF, 0xFF, 
				0x00, 0x11, 0x00, 0x00, 
				0xFF, 0xFF, 0xFF, 0xFF, 
				0x00, 0x00, 0x00
			};

			// 1st test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf32_StateInit(&utf32State, true);

			allReadCount = 0;

			while ((parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				if (allReadCount >= sizeof(result) / sizeof(UnicodeCodePoint))
				{
					fprintf(stderr, "Buffer overflow detected in line %u of file %s. Exiting.", __LINE__, __FILE__);
					exit(EXIT_FAILURE);
				}

				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));
			
			// 2nd test run

			memoryByteStream_v4ReadStateInit(&readState, &singleByte, 1, 1, 
				(bool) triggerAsEarlyAsPossible, false);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf32_StateReset(&utf32State);

			for (idx = 0; idx < sizeof(bufferIn); idx++)
			{
				singleByte = bufferIn[idx];

				parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
					&writeState, writeInterface);

				test(ParseBlocker_Reader == parseBlocker);
				test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
					writeState.blockSize * writeState.currentBufferBlockIndex));

				memoryByteStream_v4StateReset(&readState);
			}

			readInterface.commonByteStreamInterface.mpfSetStatus
				(&readState, ByteStreamStatus_Terminated);

			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
				writeState.blockSize * writeState.currentBufferBlockIndex));

			test(writeState.blockSize * writeState.currentBufferBlockIndex == 
				sizeof(result));

			// 3rd test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf32_StateReset(&utf32State);

			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
			test(!memcmp(result, bufferOut, sizeof(result)));

			// Testing behaviour after termination
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);
			test(ParseBlocker_Neither == parseBlocker);
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
		}

		// Little Endian
		{
			const uint8_t bufferIn[] = {
				0x00, 0x00, 0x00, 0x00, 
				0xFF, 0xD7, 0x00, 0x00, 
				0x00, 0xD8, 0x00, 0x00, 
				0xFF, 0xDF, 0x00, 0x00, 
				0x00, 0xE0, 0x00, 0x00, 
				0xFF, 0xFF, 0x10, 0x00, 
				0x00, 0x00, 0x11, 0x00, 
				0xFF, 0xFF, 0xFF, 0xFF, 
				0x20, 0x00, 0x00
			};

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, &singleCodePoint, 4, 1, false);

			utf32_StateInit(&utf32State, false);

			allReadCount = 0;

			while ((parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface)) != ParseBlocker_Neither)
			{
				test(ParseBlocker_Writer == parseBlocker);
				memoryByteStream_v4StateReset(&writeState);

				if (allReadCount >= sizeof(result) / sizeof(UnicodeCodePoint))
				{
					fprintf(stderr, "Buffer overflow detected in line %u of file %s. Exiting.", __LINE__, __FILE__);
					exit(EXIT_FAILURE);
				}

				test(result[allReadCount] == singleCodePoint);

				allReadCount++;
			}

			test(1 == writeState.currentBufferBlockIndex);
			allReadCount++;

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&writeState));

			test(sizeof(result) == allReadCount*sizeof(UnicodeCodePoint));
			
			// 2nd test run

			memoryByteStream_v4ReadStateInit(&readState, &singleByte, 1, 1, 
				(bool) triggerAsEarlyAsPossible, false);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf32_StateReset(&utf32State);

			for (idx = 0; idx < sizeof(bufferIn); idx++)
			{
				singleByte = bufferIn[idx];

				parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
					&writeState, writeInterface);

				test(ParseBlocker_Reader == parseBlocker);
				test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
					writeState.blockSize * writeState.currentBufferBlockIndex));

				memoryByteStream_v4StateReset(&readState);
			}

			readInterface.commonByteStreamInterface.mpfSetStatus
				(&readState, ByteStreamStatus_Terminated);

			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(!memcmp(writeState.rwBuffer.writeBuffer, result, 
				writeState.blockSize * writeState.currentBufferBlockIndex));

			test(writeState.blockSize * writeState.currentBufferBlockIndex == 
				sizeof(result));

			// 3rd test run

			memoryByteStream_v4ReadStateInit(&readState, bufferIn, 1, sizeof(bufferIn), 
				(bool) triggerAsEarlyAsPossible, true);
			memoryByteStream_v4WriteStateInit(&writeState, bufferOut, 4, 
				sizeof(bufferOut) / sizeof(UnicodeCodePoint), false);
			utf32_StateReset(&utf32State);

			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);

			test(ParseBlocker_Neither == parseBlocker);

			test(readState.currentBufferBlockIndex == readState.bufferBlockCount);
			test(ByteStreamStatus_Terminated == memoryByteStreamReadInterface_v4_get().
				commonByteStreamInterface.mpfGetStatus(&readState));
			test(ByteStreamStatus_Terminated == 
				writeInterface.commonByteStreamInterface.mpfGetStatus(&writeState));
			test(!memcmp(result, bufferOut, sizeof(result)));

			// Testing behaviour after termination
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
			parseBlocker = utf32_parse(&utf32State, &readState, readInterface, 
				&writeState, writeInterface);
			test(ParseBlocker_Neither == parseBlocker);
			test(writeState.currentBufferBlockIndex * writeState.blockSize == sizeof(result));
		}
	}
}

void test_Unicode_Parser()
{
	printf("Testing Unicode Parser UTF8\n");
	test_Unicode_Parser_UTF8();

	printf("Testing Unicode Parser UTF16\n");
	test_Unicode_Parser_UTF16();

	printf("Testing Unicode Parser UTF32\n");
	test_Unicode_Parser_UTF32();
}
