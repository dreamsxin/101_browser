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
#include "Unicode/Parser_Util.h"
#include "MiniStdlib/MTAx_cstdlib.h" // for the conversation functions for endianness
#include <assert.h>

void utf32_StateInit(UTF32_State *out_pState, bool in_bigEndian)
{
	out_pState->bigEndian = in_bigEndian;
	utf32_StateReset(out_pState);
}

void utf32_StateReset(UTF32_State *out_pState)
{
	out_pState->entryPoint = UTF32_EntryPoint_BeforeReading;
	out_pState->readCount = 0;
}

ParseBlocker utf32_parse(
	void *in_out_pParserState, 
	void *in_pReadState, ByteStreamReadInterface_v4 in_readInterface, 
	void *in_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface)
{
	UTF32_State *pUTF32State = (UTF32_State *) in_out_pParserState;
	extern const UnicodeCodePoint cReplacementCharacter;

	assert(in_readInterface.mpfRead != NULL);
	assert(in_readInterface.commonByteStreamInterface.mpfGetStatus != NULL);

	switch (pUTF32State->entryPoint)
	{
	case UTF32_EntryPoint_BeforeReading:
		goto Label_EntryPoint_BeforeReading;
	case UTF32_EntryPoint_WriteTerminalReplacementCharacter:
		goto Label_EntryPoint_WriteTerminalReplacementCharacter;
	case UTF32_EntryPoint_WriteCharacter:
		goto Label_EntryPoint_WriteCharacter;
	case UTF32_EntryPoint_Terminated:
		goto Label_EntryPoint_Terminated;
	}

	assert(false);

	while (1)
	{
		size_t rwCount;

		pUTF32State->readCount = 0;

Label_EntryPoint_BeforeReading:
		assert(pUTF32State->readCount < 4);

		rwCount = in_readInterface.mpfRead(in_pReadState, 
			((uint8_t*) &pUTF32State->currentCodePoint)+pUTF32State->readCount, 
			4 - pUTF32State->readCount);

		assert(rwCount <= 4u - pUTF32State->readCount);

		pUTF32State->readCount += (uint8_t) rwCount;

		if (0 == pUTF32State->readCount)
		{
			ByteStreamStatus_v4 status = in_readInterface.
				commonByteStreamInterface.mpfGetStatus(in_pReadState);
			assert(ByteStreamStatus_OK != status);

			if (ByteStreamStatus_Terminated == status)
				goto terminate;
			else
			{
				pUTF32State->entryPoint = UTF32_EntryPoint_BeforeReading;
				return ParseBlocker_Reader;
			}
		}
		else if (4 != pUTF32State->readCount)
		{
			ByteStreamStatus_v4 status = in_readInterface.
				commonByteStreamInterface.mpfGetStatus(in_pReadState);
			assert(ByteStreamStatus_OK != status);
			
			assert(pUTF32State->readCount < 4);

			if (ByteStreamStatus_Terminated == status)
			{
Label_EntryPoint_WriteTerminalReplacementCharacter:
				// Write terminal replacement character
				if (emitCodepoint(in_pWriteState, in_writeInterface, 
					cReplacementCharacter, &pUTF32State->entryPoint, 
					sizeof(UTF32_EntryPoint), 
					UTF32_EntryPoint_WriteCharacter))
					return ParseBlocker_Writer;

				goto terminate;
			}
			else
			{
				pUTF32State->entryPoint = UTF32_EntryPoint_BeforeReading;
				return ParseBlocker_Reader;
			}
		}

		if (pUTF32State->bigEndian)
			pUTF32State->currentCodePoint = _byteswap_ulong(pUTF32State->currentCodePoint);

		if ((pUTF32State->currentCodePoint >= 0xD800 && 
			pUTF32State->currentCodePoint <= 0xDFFF) ||
			pUTF32State->currentCodePoint >= 0x110000)
			pUTF32State->currentCodePoint = cReplacementCharacter;

Label_EntryPoint_WriteCharacter:
		if (emitCodepoint(in_pWriteState, in_writeInterface, 
			pUTF32State->currentCodePoint, &pUTF32State->entryPoint, 
			sizeof(UTF32_EntryPoint), 
			UTF32_EntryPoint_WriteCharacter))
			return ParseBlocker_Writer;
	}

terminate:
	pUTF32State->entryPoint = UTF32_EntryPoint_Terminated;

Label_EntryPoint_Terminated:
	assert(ByteStreamStatus_Terminated == in_readInterface.
		commonByteStreamInterface.mpfGetStatus(in_pReadState));
	return ParseBlocker_Neither;
}
