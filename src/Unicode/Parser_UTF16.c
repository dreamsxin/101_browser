/*
* Copyright 2012 Wolfgang Keller
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

#include "Unicode/Parser.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/MTAx_cstdlib.h" // for the conversation functions for endianness
#include <assert.h>

ReadResult parse_UTF16(
	ByteStreamReadInterface_v3 in_readInterface, 
	void *in_pReadState,
	ByteStreamWriteInterface_v3 in_writeInterface,
	void *in_pWriteState, 
	bool in_bigEndian)
{
	UnicodeCodePoint currentCodePoint;
	size_t rwCount;
	extern const UnicodeCodePoint cReplacementCharacter;
	/*
	* This variable is set to true, if we shall parse a second
	* byte of a surrogate.
	*/
	bool lStateIsSecondByte = false;

	assert(in_readInterface.mpfRead != NULL);
	assert(in_writeInterface.mpfWrite != NULL);

	if (in_readInterface.commonByteStreamInterface.mpfIsTerminated(in_pReadState))
		goto terminate;

	while (1)
	{
		uint16_t currentWord;

		rwCount = in_readInterface.mpfRead(in_pReadState, &currentWord, 2);

		assert(rwCount <= 2);

		if (0 == rwCount)
		{
			assert(in_readInterface.commonByteStreamInterface.mpfIsTerminated(in_pReadState));

			if (!lStateIsSecondByte)
				goto terminate;
			else
				goto write_terminal_replacement_character;
		}
		else if (1 == rwCount)
		{
			assert(in_readInterface.commonByteStreamInterface.mpfIsTerminated(in_pReadState));

			if (!lStateIsSecondByte)
			{
				goto write_terminal_replacement_character;
			}
			else
			{
				if (currentWord >= 0xDC && currentWord <= 0xDF)
					goto write_terminal_replacement_character;
				else
				{
					UnicodeCodePoint codepoints[2] = { cReplacementCharacter, 
						cReplacementCharacter };

					rwCount = in_writeInterface.mpfWrite(in_pWriteState, codepoints, 
						sizeof(codepoints));

					if (sizeof(codepoints) != rwCount)
					{
						assert(in_writeInterface.commonByteStreamInterface.mpfIsTerminated(in_pWriteState));
						return ReadResultWriteError;
					}
					else
					{
						in_writeInterface.commonByteStreamInterface.mpfTerminate(in_pWriteState);
						return ReadResultOK;
					}
				}
			}
		}
		else
		{
			assert(2 == rwCount);

			if (in_bigEndian)
				currentWord = _byteswap_ushort(currentWord);

			if (!lStateIsSecondByte)
			{
begin_of_S:
				if (currentWord < 0xD800 || currentWord >= 0xDC00)
				{
					// 0xDC00 <= currentWord < 0xE000: low surrogate
					if (0xDC00 <= currentWord  && currentWord < 0xE000)
					{
						assert(currentWord >= 0xDC00);
						currentCodePoint = cReplacementCharacter;
					}
					else
						currentCodePoint = currentWord;

					if (in_readInterface.commonByteStreamInterface.mpfIsTerminated(in_pReadState))
						goto write_terminal_character;

					rwCount = in_writeInterface.mpfWrite(in_pWriteState, &currentCodePoint, 
						sizeof(UnicodeCodePoint));

					if (sizeof(UnicodeCodePoint) != rwCount)
						return ReadResultWriteError;
				}
				else if (currentWord < 0xDC00)
				{
					assert(currentWord >= 0xD800);
					assert(currentWord <= 0xDBFF);

					if (in_readInterface.commonByteStreamInterface.mpfIsTerminated(in_pReadState))
						goto write_terminal_replacement_character;

					currentCodePoint = (currentWord & 0x3FFu) << 10u;
					lStateIsSecondByte = true;
				}
			}
			else
			{
				lStateIsSecondByte = false;

				if (currentWord < 0xDC00 || 0xDFFF < currentWord)
				{
					currentCodePoint = cReplacementCharacter;

					rwCount = in_writeInterface.mpfWrite(in_pWriteState, &currentCodePoint, 
						sizeof(UnicodeCodePoint));

					if (sizeof(UnicodeCodePoint) != rwCount)
						return ReadResultWriteError;

					goto begin_of_S;
				}
				else
				{
					currentCodePoint |= (currentWord & 0x3FFu);
					currentCodePoint += 0x10000;
				}

				if (in_readInterface.commonByteStreamInterface.mpfIsTerminated(in_pReadState))
					goto write_terminal_character;

				rwCount = in_writeInterface.mpfWrite(in_pWriteState, &currentCodePoint, 
					sizeof(UnicodeCodePoint));

				if (sizeof(UnicodeCodePoint) != rwCount)
					return ReadResultWriteError;
			}
		}
	}

write_terminal_replacement_character:
	currentCodePoint = cReplacementCharacter;
write_terminal_character:
	rwCount = in_writeInterface.mpfWrite(in_pWriteState, &currentCodePoint, 
		sizeof(UnicodeCodePoint));

	if (sizeof(UnicodeCodePoint) != rwCount)
		return ReadResultWriteError;
	// Otherwise continue and terminate

terminate:
	in_writeInterface.commonByteStreamInterface.mpfTerminate(in_pWriteState);
	return ReadResultOK;
}
