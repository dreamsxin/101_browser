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

void utf16_StateInit(UTF16_State *out_pState, 
	ByteStreamReadInterface_v3 in_readInterface, 
	void *in_pReadState, 
	bool in_bigEndian)
{
	out_pState->parserState.pReadState = in_pReadState;
	out_pState->parserState.readInterface = in_readInterface;
	out_pState->bigEndian = in_bigEndian;
	
	utf16_StateReset(out_pState);
}

void utf16_StateReset(UTF16_State *out_pState)
{
	out_pState->isSecondByte = false;
	out_pState->currentLabel = UTF16_CurrentLabel_Begin;
	// No necessity to initialize/reset currentWord
}

ByteStreamReadInterface_v3 getUTF16_ReadInterface()
{
	return getParser_ReadInterface(utf16_read);
}

size_t utf16_read(
	void *in_out_pByteStreamState, 
	void *out_pBuffer, size_t in_count)
{
	UTF16_State *pUTF16State = (UTF16_State *) in_out_pByteStreamState;
	size_t writeCount = 0;
	extern const UnicodeCodePoint cReplacementCharacter;

	assert(pUTF16State->parserState.readInterface.mpfRead != NULL);
	assert(pUTF16State->parserState.readInterface.commonByteStreamInterface.mpfIsTerminated != NULL);

	/*
	* Q: Why do we test this before and not in the loop
	*    (kind of while (writeCount != in_count))?
	* A: If we jump to StateLabel_BeginOfS or 
	*    StateLabel_WriteTerminalReplacementCharacter we require
	*    in_count != 0 to avoid a buffer overflow.
	*/
	if (0 == in_count)
		return 0;

	switch (pUTF16State->currentLabel)
	{
	case UTF16_CurrentLabel_Begin:
		goto StateLabel_Begin;
	case UTF16_CurrentLabel_BeginOfS:
		goto StateLabel_BeginOfS;
	case UTF16_CurrentLabel_WriteTerminalReplacementCharacter:
		goto StateLabel_WriteTerminalReplacementCharacter;
	default:
		assert(false);
	}

	do
	{
		size_t rwCount;
		UnicodeCodePoint currentCodePoint;

StateLabel_Begin:
		assert(writeCount < in_count);

		rwCount = pUTF16State->parserState.readInterface.mpfRead(
			pUTF16State->parserState.pReadState, 
			&pUTF16State->currentWord, 2);

		assert(rwCount <= 2);

		if (0 == rwCount)
		{
			assert(pUTF16State->parserState.readInterface.commonByteStreamInterface.
				mpfIsTerminated(pUTF16State->parserState.pReadState));

			if (!pUTF16State->isSecondByte)
				goto terminate;
			else
				goto StateLabel_WriteTerminalReplacementCharacter;
		}
		else if (1 == rwCount)
		{
			assert(pUTF16State->parserState.readInterface.commonByteStreamInterface.
				mpfIsTerminated(pUTF16State->parserState.pReadState));

			if (!pUTF16State->isSecondByte)
			{
				goto StateLabel_WriteTerminalReplacementCharacter;
			}
			else
			{
				/*
				* Q: Why this line?
				* A: Because we read only one byte (the lower one), we don't 
				*    know what the value of the higher byte is. It can be
				*    anything.
				*/
				pUTF16State->currentWord = pUTF16State->currentWord & 0xFF;

				if (pUTF16State->currentWord >= 0xDC && 
					pUTF16State->currentWord <= 0xDF)
					goto StateLabel_WriteTerminalReplacementCharacter;
				else
				{
					writeCodePoint((UnicodeCodePoint**) &out_pBuffer, &writeCount, cReplacementCharacter);

					if (writeCount == in_count)
					{
						pUTF16State->currentLabel = UTF16_CurrentLabel_WriteTerminalReplacementCharacter;
						return writeCount;
					}
					else
					{
						assert(writeCount < in_count);
						goto StateLabel_WriteTerminalReplacementCharacter;
					}
				}
			}
		}
		else
		{
			assert(2 == rwCount);

			if (pUTF16State->bigEndian)
				pUTF16State->currentWord = _byteswap_ushort(pUTF16State->currentWord);

			if (!pUTF16State->isSecondByte)
			{
StateLabel_BeginOfS:
				if (pUTF16State->currentWord < 0xD800 || 
					pUTF16State->currentWord >= 0xDC00)
				{
					// 0xDC00 <= currentWord < 0xE000: low surrogate
					if (0xDC00 <= pUTF16State->currentWord  && 
						pUTF16State->currentWord < 0xE000)
						currentCodePoint = cReplacementCharacter;
					else
						currentCodePoint = pUTF16State->currentWord;
				}
				else
				{
					assert(pUTF16State->currentWord >= 0xD800);
					assert(pUTF16State->currentWord < 0xDC00);
					assert(pUTF16State->currentWord <= 0xDBFF);

					currentCodePoint = (pUTF16State->currentWord & 0x3FFu) << 10u;
					pUTF16State->isSecondByte = true;

					continue;
				}
			}
			else
			{
				pUTF16State->isSecondByte = false;

				if (pUTF16State->currentWord < 0xDC00 || 
					0xDFFF < pUTF16State->currentWord)
				{
					currentCodePoint = cReplacementCharacter;

					writeCodePoint((UnicodeCodePoint**) &out_pBuffer, &writeCount, currentCodePoint);

					if (writeCount == in_count)
					{
						pUTF16State->currentLabel = UTF16_CurrentLabel_BeginOfS;
						return writeCount;
					}
					else
					{
						assert(writeCount < in_count);
						goto StateLabel_BeginOfS;
					}
				}
				else
				{
					currentCodePoint |= (pUTF16State->currentWord & 0x3FFu);
					currentCodePoint += 0x10000;
				}
			}

			writeCodePoint((UnicodeCodePoint**) &out_pBuffer, &writeCount, currentCodePoint);
		}
	} while (writeCount != in_count);

	goto terminate;

StateLabel_WriteTerminalReplacementCharacter:
	assert(writeCount < in_count);
	writeCodePoint((UnicodeCodePoint**) &out_pBuffer, &writeCount, cReplacementCharacter);

terminate:
	pUTF16State->currentLabel = UTF16_CurrentLabel_Begin;
	return writeCount;
}
