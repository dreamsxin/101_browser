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
#include "MiniStdlib/MTAx_cstdlib.h" // for the conversation functions for endianness
#include <assert.h>
#include <string.h> // memset

void utf32_StateInit(UTF32_State *out_pState, 
	ByteStreamReadInterface_v3 in_readInterface, 
	void *in_pReadState, 
	bool in_bigEndian)
{
	out_pState->parserState.readInterface = in_readInterface;
	out_pState->parserState.pReadState = in_pReadState;
	out_pState->bigEndian = in_bigEndian;
}

void utf32_StateReset(UTF32_State *out_pState)
{
	// Nothing
}

bool utf32_isTerminated(const void *in_pByteStreamState)
{
	assert(((UTF32_State *) in_pByteStreamState)->parserState.readInterface.
		commonByteStreamInterface.mpfIsTerminated != NULL);

	return ((UTF32_State *) in_pByteStreamState)->parserState.readInterface.
		commonByteStreamInterface.mpfIsTerminated(
		((UTF32_State *) in_pByteStreamState)->parserState.pReadState);
}

void utf32_Terminate(void *in_out_pByteStreamState)
{
	assert(((UTF32_State *) in_out_pByteStreamState)->parserState.readInterface.
		commonByteStreamInterface.mpfTerminate != NULL);

	((UTF32_State *) in_out_pByteStreamState)->parserState.readInterface.
		commonByteStreamInterface.mpfTerminate(
		((UTF32_State *) in_out_pByteStreamState)->parserState.pReadState);
}

ByteStreamReadInterface_v3 getUTF32_ReadInterface()
{
	ByteStreamReadInterface_v3 out_interface;
	memset(&out_interface, 0, sizeof(out_interface));
	out_interface.commonByteStreamInterface.mpfIsTerminated = utf32_isTerminated;
	out_interface.commonByteStreamInterface.mpfTerminate = utf32_Terminate;
	out_interface.mpfRead = utf32_read;

	return out_interface;
}

size_t utf32_read(
	void *in_out_pByteStreamState, 
	void *out_pBuffer, size_t in_count)
{
	UTF32_State *pUTF32State = (UTF32_State *) in_out_pByteStreamState;
	UnicodeCodePoint currentCodePoint;
	size_t writeCount = 0;
	extern const UnicodeCodePoint cReplacementCharacter;

	assert(pUTF32State->parserState.readInterface.mpfRead != NULL);
	assert(pUTF32State->parserState.readInterface.commonByteStreamInterface.mpfIsTerminated != NULL);
	
	if (pUTF32State->parserState.readInterface.commonByteStreamInterface.
		mpfIsTerminated(pUTF32State->parserState.pReadState))
		goto terminate;

	while (writeCount != in_count)
	{
		size_t rwCount;
		assert(writeCount < in_count);
		
		rwCount = pUTF32State->parserState.readInterface.mpfRead(
			pUTF32State->parserState.pReadState, &currentCodePoint, 4);

		assert(rwCount <= 4);

		if (0 == rwCount)
		{
			assert(pUTF32State->parserState.readInterface.commonByteStreamInterface.mpfIsTerminated(
				pUTF32State->parserState.pReadState));

			goto terminate;
		}
		else if (4 != rwCount)
		{
			assert(rwCount > 0);
			assert(rwCount < 4);
			assert(pUTF32State->parserState.readInterface.commonByteStreamInterface.mpfIsTerminated(
				pUTF32State->parserState.pReadState));

			currentCodePoint = cReplacementCharacter;
			goto write_terminal_character;
		}

		if (pUTF32State->bigEndian)
			currentCodePoint = _byteswap_ulong(currentCodePoint);

		if ((currentCodePoint >= 0xD800 && currentCodePoint <= 0xDFFF) ||
			currentCodePoint >= 0x110000)
			currentCodePoint = cReplacementCharacter;

		if (pUTF32State->parserState.readInterface.commonByteStreamInterface.
			mpfIsTerminated(pUTF32State->parserState.pReadState))
			goto write_terminal_character;

		*(UnicodeCodePoint *) out_pBuffer = currentCodePoint;
		writeCount++;
		out_pBuffer = (UnicodeCodePoint *) out_pBuffer + 1;
	}

	goto terminate;

write_terminal_character:
	*(UnicodeCodePoint *) out_pBuffer = currentCodePoint;
	writeCount++;

	// Otherwise continue and terminate

terminate:
	return writeCount;
}
