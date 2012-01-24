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


#include "Unicode/Parser.h"
#include "Algorithm/Bitmask.h"
#include "MiniStdlib/cstdint.h"
#include <assert.h>

typedef enum
{
	UTF8ParseState_Start,

	UTF8ParseState_X_error,
	UTF8ParseState_X_X_error,
	UTF8ParseState_X_X_X_error,
	UTF8ParseState_X_X_X_X_error,
	UTF8ParseState_X_X_X_X_X_error,

	UTF8ParseState_X_append_emit,
	UTF8ParseState_X_X_append_emit,
	UTF8ParseState_X_X_X_append_emit,

	UTF8ParseState_E0,
	UTF8ParseState_ED,
	UTF8ParseState_F0,
	UTF8ParseState_F4
} UTF8ParseState;

const uint8_t cBitmaskIdentifierFirst1Byte = 0x80;
const uint8_t cBitmaskValueFirst1Byte = 0x0;

const uint8_t cBitmaskIdentifierFirst2Bytes = 0xE0;
const uint8_t cBitmaskValueFirst2Bytes = 0xA0;

const uint8_t cBitmaskIdentifierFirst3Bytes = 0xF0;
const uint8_t cBitmaskValueFirst3Bytes = 0xE0;

const uint8_t cBitmaskIdentifierFirst4Bytes = 0xF8;
const uint8_t cBitmaskValueFirst4Bytes = 0xF0;

const uint8_t cBitmaskIdentifierFollowingByte = 0xC0;
const uint8_t cBitmaskValueFollowingByte = 0x80;

UnicodeCodePoint createCodePoint(uint8_t in_bytes[], uint8_t in_count)
{
	UnicodeCodePoint codePoint;
	uint8_t idx;

	assert(in_count >= 1);
	assert(in_count <= 4);

	if (in_count == 1)
	{
		assert(checkBitmask(in_bytes+0, 1, &cBitmaskIdentifierFirst1Byte, &cBitmaskValueFirst1Byte));

		codePoint = in_bytes[0];
	}
	else
	{
		assert(in_count >= 2);
		assert(in_count <= 4);

		codePoint = in_bytes[0] & (0xFF >> (in_count+3));

		if (in_count == 2)
			assert(checkBitmask(in_bytes+0, 1, &cBitmaskIdentifierFirst2Bytes, &cBitmaskValueFirst2Bytes));
		else if (in_count == 3)
			assert(checkBitmask(in_bytes+0, 1, &cBitmaskIdentifierFirst3Bytes, &cBitmaskValueFirst3Bytes));
		else if (in_count == 4)
			assert(checkBitmask(in_bytes+0, 1, &cBitmaskIdentifierFirst4Bytes, &cBitmaskValueFirst4Bytes));

		for (idx = 1; idx < in_count; idx++)
		{
			assert(checkBitmask(in_bytes+idx, 1, &cBitmaskIdentifierFirst1Byte, &cBitmaskValueFollowingByte));

			codePoint <<= 6;
			codePoint |= (in_bytes[idx] & 0x3F);
		}

		assert(codePoint <= 0x10FFFF);
		assert(codePoint < 0xD800 || codePoint > 0xDFFF);
	}

	return codePoint;
}

bool instr_append(
	UTF8ParseState *out_pParserState, 
	uint8_t in_currentByte, 
	uint8_t in_out_pReadBytes[], uint8_t *in_out_pReadBytesCount,
	UTF8ParseState in_nextParserState)
{
	assert(*in_out_pReadBytesCount < 3);

	in_out_pReadBytes[*in_out_pReadBytesCount] = in_currentByte;
	(*in_out_pReadBytesCount)++;

	*out_pParserState = in_nextParserState;

	return true;
}

bool instr_append_emit(ByteStreamInterface in_writeInterface, void *in_pWriteState, 
	UTF8ParseState *out_pParserState, 
	uint8_t in_currentByte, 
	uint8_t in_out_pReadBytes[], uint8_t *in_out_pReadBytesCount)
{
	UnicodeCodePoint codePoint;
	size_t bytesWritten;

	assert(in_writeInterface.mpfWrite != NULL);

	instr_append(out_pParserState, in_currentByte, in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_Start);

	codePoint = createCodePoint(in_out_pReadBytes, *in_out_pReadBytesCount);

	bytesWritten = in_writeInterface.mpfWrite(in_pWriteState, &codePoint, sizeof(UnicodeCodePoint));

	*in_out_pReadBytesCount = 0;

	return bytesWritten == sizeof(UnicodeCodePoint);
}

bool instr_error(ByteStreamInterface in_writeInterface, void *in_pWriteState, 
	UTF8ParseState *out_pParserState, uint8_t *out_pReadBytesCount)
{
	UnicodeCodePoint replacementCharacter = 0xFFFD;

	size_t bytesWritten = (*in_writeInterface.mpfWrite)(in_pWriteState, &replacementCharacter, sizeof(UnicodeCodePoint));
	
	assert(in_writeInterface.mpfWrite != NULL);

	*out_pParserState = UTF8ParseState_Start;
	*out_pReadBytesCount = 0;
	
	return bytesWritten == sizeof(UnicodeCodePoint);
}

bool instr_action_of_Start(ByteStreamInterface in_writeInterface, void *in_pWriteState, 
	UTF8ParseState *out_pParserState, 
	uint8_t in_currentByte, 
	uint8_t in_out_pReadBytes[], uint8_t *in_out_pReadBytesCount)
{
	if (in_currentByte <= 0x7F)
	{
		return instr_append_emit(in_writeInterface, in_pWriteState, out_pParserState, 
			in_currentByte, in_out_pReadBytes, in_out_pReadBytesCount);
	}
	else if (in_currentByte <= 0xBF)
	{
		return instr_error(in_writeInterface, in_pWriteState, out_pParserState, in_out_pReadBytesCount);
	}
	else if (in_currentByte <= 0xC1)
	{
		*out_pParserState = UTF8ParseState_X_error;
		return true;
	}
	else if (in_currentByte <= 0xDF)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_append_emit);
	}
	else if (in_currentByte == 0xE0)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_E0);
	}
	else if (in_currentByte <= 0xEC)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_X_append_emit);
	}
	else if (in_currentByte == 0xED)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_ED);
	}
	else if (in_currentByte <= 0xEF)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_X_append_emit);
	}
	else if (in_currentByte == 0xF0)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_F0);
	}
	else if (in_currentByte <= 0xF3)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_X_X_append_emit);
	}
	else if (in_currentByte == 0xF4)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_F4);
	}
	else if (in_currentByte <= 0xF7)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_X_X_error);
	}
	else if (in_currentByte <= 0xFB)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_X_X_X_error);
	}
	else if (in_currentByte <= 0xFD)
	{
		return instr_append(out_pParserState, 
			in_currentByte,  in_out_pReadBytes, in_out_pReadBytesCount, UTF8ParseState_X_X_X_X_X_error);
	}
	else
	{
		assert(in_currentByte == 0xFE || in_currentByte == 0xFF);
		return instr_error(in_writeInterface, in_pWriteState, out_pParserState, in_out_pReadBytesCount);
	}
}


ReadResult parse_UTF8(
	ByteStreamInterface in_readInterface, 
	void *in_pReadState,
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState)
{
	UTF8ParseState parserState = UTF8ParseState_Start;
	
	uint8_t readBytes[4];
	uint8_t readBytesCount = 0;

	uint8_t currentByte;
	bool result;
	
	assert(in_readInterface.mpfRead != NULL);
	assert(in_writeInterface.mpfWrite != NULL);

	while (1)
	{
		size_t currentReadCount = (*in_readInterface.mpfRead)(in_pReadState, &currentByte, 1);

		if (currentReadCount == 0)
		{
			/*
			* Situation: the input stream stopped in the middle of a codepoint
			* (if we had (UTF8ParseState_Start == parserState) we'd stop at the 
			* end of a codepoint - perfectly OK).
			*/
			if (parserState != UTF8ParseState_Start)
			{
				result = instr_error(in_writeInterface, in_pWriteState, &parserState, &readBytesCount);

				/*
				* Note that because a replacement character is written, 
				* we return only an error if it could not be written.
				*/
				if (!result)
					return ReadResultWriteError;
				else
					return ReadResultOK;
			}
			else
				return ReadResultOK;
		}

		if (parserState == UTF8ParseState_Start)
		{
			result = instr_action_of_Start(in_writeInterface, in_pWriteState, 
				&parserState, currentByte, readBytes, &readBytesCount);

			if (!result)
				return ReadResultWriteError;
		}
		else
		{
			if (currentByte <= 0x7F || currentByte >= 0xC0)
			{
				result = instr_error(in_writeInterface, in_pWriteState, &parserState, &readBytesCount);

				if (!result)
					return ReadResultWriteError;

				result = instr_action_of_Start(in_writeInterface, in_pWriteState, &parserState, currentByte, readBytes, &readBytesCount);

				if (!result)
					return ReadResultWriteError;
			}
			else
			{
				assert(currentByte >= 0x80);
				assert(currentByte <= 0xBF);

				if (UTF8ParseState_X_error == parserState)
				{
					result = instr_error(in_writeInterface, in_pWriteState, &parserState, &readBytesCount);

					if (!result)
						return ReadResultWriteError;
				}
				else if (UTF8ParseState_X_X_error == parserState)
				{
					parserState = UTF8ParseState_X_error;
				}
				else if (UTF8ParseState_X_X_X_error == parserState)
				{
					parserState = UTF8ParseState_X_X_error;
				}
				else if (UTF8ParseState_X_X_X_X_error == parserState)
				{
					parserState = UTF8ParseState_X_X_X_error;
				}
				else if (UTF8ParseState_X_X_X_X_X_error == parserState)
				{
					parserState = UTF8ParseState_X_X_X_X_error;
				}
				else if (UTF8ParseState_X_append_emit == parserState)
				{
					result = instr_append_emit(in_writeInterface, in_pWriteState, &parserState, 
						currentByte, readBytes, &readBytesCount);

					if (!result)
						return ReadResultWriteError;
				}
				else if (UTF8ParseState_X_X_append_emit == parserState)
				{
					result = instr_append(&parserState, 
						currentByte, readBytes, &readBytesCount, UTF8ParseState_X_append_emit);

					if (!result)
						return ReadResultWriteError;
				}
				else if (UTF8ParseState_X_X_X_append_emit == parserState)
				{
					result = instr_append(&parserState, 
						currentByte, readBytes, &readBytesCount, UTF8ParseState_X_X_append_emit);

					if (!result)
						return ReadResultWriteError;
				}
				else if (UTF8ParseState_E0 == parserState)
				{
					if (currentByte <= 0x9F)
					{
						parserState = UTF8ParseState_X_error;
					}
					else
					{
						result = instr_append(&parserState, 
						currentByte, readBytes, &readBytesCount, UTF8ParseState_X_append_emit);

						if (!result)
							return ReadResultWriteError;
					}
				}
				else if (UTF8ParseState_ED == parserState)
				{
					if (currentByte > 0x9F)
					{
						parserState = UTF8ParseState_X_error;
					}
					else
					{
						result = instr_append(&parserState, 
						currentByte, readBytes, &readBytesCount, UTF8ParseState_X_append_emit);

						if (!result)
							return ReadResultWriteError;
					}
				}
				else if (UTF8ParseState_F0 == parserState)
				{
					if (currentByte <= 0x8F)
					{
						parserState = UTF8ParseState_X_X_error;
					}
					else
					{
						result = instr_append(&parserState, 
						currentByte, readBytes, &readBytesCount, UTF8ParseState_X_X_append_emit);

						if (!result)
							return ReadResultWriteError;
					}
				}
				else if (UTF8ParseState_F4 == parserState)
				{
					if (currentByte > 0x8F)
					{
						parserState = UTF8ParseState_X_X_error;
					}
					else
					{
						result = instr_append(&parserState, 
						currentByte, readBytes, &readBytesCount, UTF8ParseState_X_X_append_emit);

						if (!result)
							return ReadResultWriteError;
					}
				}
			}
		}
	}
}
