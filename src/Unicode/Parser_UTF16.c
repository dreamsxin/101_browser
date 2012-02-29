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
	out_pState->currentLabel = UTF16_CurrentLabel_Begin;
	out_pState->isSecondWord = false;
	// No necessity to initialize/reset prevWord
}

ByteStreamReadInterface_v3 getUTF16_ReadInterface()
{
	return getParser_ReadInterface(utf16_read);
}

// See http://unicode.org/faq/utf_bom.html#utf16-4
const UnicodeCodePoint SURROGATE_OFFSET = 0x10000 - (0xD800 << 10) - 0xDC00;

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
	case UTF16_CurrentLabel_HandleAfterHighSurrogate:
		goto StateLabel_HandleAfterHighSurrogate;
	case UTF16_CurrentLabel_WriteTerminalReplacementCharacter:
		goto StateLabel_WriteTerminalReplacementCharacter;
	default:
		assert(false);
	}

	do
	{
		size_t rwCount;
		uint16_t currentWord;

StateLabel_Begin:
		assert(writeCount < in_count);

		rwCount = pUTF16State->parserState.readInterface.mpfRead(
			pUTF16State->parserState.pReadState, 
			&currentWord, 2);

		assert(rwCount <= 2);

		if (0 == rwCount)
		{
			assert(pUTF16State->parserState.readInterface.commonByteStreamInterface.
				mpfIsTerminated(pUTF16State->parserState.pReadState));

			if (!pUTF16State->isSecondWord)
				goto terminate;
			else
				goto StateLabel_WriteTerminalReplacementCharacter;
		}
		else if (1 == rwCount)
		{
			assert(pUTF16State->parserState.readInterface.commonByteStreamInterface.
				mpfIsTerminated(pUTF16State->parserState.pReadState));

			if (!pUTF16State->isSecondWord || !pUTF16State->bigEndian || 
				/*
				* Q: Why the bitwise and with 0xFF?
				* A: Because we read only one byte (the lower one), we don't 
				*    know what the value of the higher byte is. It can be
				*    anything.
				*/
				((currentWord & 0xFF) >= 0xDC && (currentWord & 0xFF) <= 0xDF))
			{
				goto StateLabel_WriteTerminalReplacementCharacter;
			}
			else
			{
				assert(pUTF16State->isSecondWord);
				assert(pUTF16State->bigEndian);
				assert((currentWord & 0xFF) < 0xDC || (currentWord & 0xFF) > 0xDF);

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
		else
		{
			UnicodeCodePoint currentCodePoint = 0xFFFFFFFF;
			assert(2 == rwCount);

			if (pUTF16State->bigEndian)
				currentWord = _byteswap_ushort(currentWord);

			if (!pUTF16State->isSecondWord)
			{
begin_of_S:
				if (currentWord < 0xD800 || currentWord >= 0xDC00)
				{
					// 0xDC00 <= currentWord < 0xE000: low surrogate
					if (0xDC00 <= currentWord  && currentWord < 0xE000)
						currentCodePoint = cReplacementCharacter;
					else
						currentCodePoint = currentWord;
				}
				else
				{
					assert(currentWord >= 0xD800);
					assert(currentWord <= 0xDBFF);

					pUTF16State->prevWord = currentWord;

					//currentCodePoint = (pUTF16State->currentWord & 0x3FFu) << 10u;
					pUTF16State->isSecondWord = true;

					continue;
				}
			}
			else
			{
				pUTF16State->isSecondWord = false;

				if (currentWord < 0xDC00 || 0xDFFF < currentWord)
				{
					writeCodePoint((UnicodeCodePoint**) &out_pBuffer, &writeCount, cReplacementCharacter);

					if (writeCount == in_count)
					{
						pUTF16State->currentLabel = UTF16_CurrentLabel_HandleAfterHighSurrogate;

						// Backup currentCodeWord
						pUTF16State->prevWord = currentWord;
						return writeCount;

StateLabel_HandleAfterHighSurrogate:
						// Restore currentCodeWord
						currentWord = pUTF16State->prevWord;
					}

					assert(writeCount < in_count);
					goto begin_of_S;
				}
				else
				{
					// See http://unicode.org/faq/utf_bom.html#utf16-4
					currentCodePoint = (pUTF16State->prevWord << 10) + currentWord + SURROGATE_OFFSET;
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
