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

ReadResult parse_UTF32(
	ByteStreamInterface in_readInterface, 
	void *in_pReadState,
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState, 
	bool in_bigEndian)
{
	assert(in_readInterface.mpfRead != NULL);
	assert(in_writeInterface.mpfWrite != NULL);

	while (1)
	{
		size_t rwCount;
		UnicodeCodePoint currentCodePoint;

		rwCount = in_readInterface.mpfRead(in_pReadState, &currentCodePoint, 4);

		if (0 == rwCount)
		{
			return terminateStream(in_writeInterface, in_pWriteState);
		}
		else if (4 != rwCount)
		{
			assert(rwCount > 0);
			assert(rwCount < 4);

			return writeTerminalReplacementCharacter(in_writeInterface, in_pWriteState);
		}

		if (in_bigEndian)
			currentCodePoint = _byteswap_ulong(currentCodePoint);

		if ((currentCodePoint >= 0xD800 && currentCodePoint <= 0xDFFF) ||
			currentCodePoint >= 0x110000)
		{
			currentCodePoint = REPLACEMENT_CHARACTER;
		}

		rwCount = (*in_writeInterface.mpfWrite)(in_pWriteState, &currentCodePoint, sizeof(UnicodeCodePoint));

		if (rwCount != sizeof(UnicodeCodePoint))
			return ReadResultWriteError;
	}
}
