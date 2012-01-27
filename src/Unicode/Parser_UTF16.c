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
	ByteStreamInterface in_readInterface, 
	void *in_pReadState,
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState, 
	bool in_bigEndian)
{
	UnicodeCodePoint currentCodePoint;
	/*
	* This variable is set to true, if we shall parse a second
	* byte of a surrogate.
	*/
	bool lStateIsSecondByte = false;

	assert(in_readInterface.mpfRead != NULL);
	assert(in_writeInterface.mpfWrite != NULL);


	while (1)
	{
		size_t rwCount;
		UnicodeCodePoint currentCodePoint;
		uint16_t currentWord;

		rwCount = in_readInterface.mpfRead(in_pReadState, &currentWord, 2);

		assert(rwCount <= 2);

		if (!lStateIsSecondByte)
		{
			if (0 == rwCount)
			{
				// Is there a need to write 0 bytes?
				return ReadResultOK;
			}
			//else if 
		}
		else
		{

		}
	}

	return ReadResultOK;
}
