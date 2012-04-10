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

#include "Unicode/Parser_Util.h"
#include <assert.h>
#include <string.h> // memcpy

const UnicodeCodePoint cReplacementCharacter = 0xFFFD;

int emitCodepoint(void *in_out_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface, 
	UnicodeCodePoint in_codePoint, void *out_pFailureEntryPointAdress, 
	size_t in_failureEntryPointSize, unsigned int in_failureEntryPointValue)
{
	size_t writeCount = in_writeInterface.mpfWrite(in_out_pWriteState, &in_codePoint, 1);

	assert(in_failureEntryPointSize > 0);
	assert(in_failureEntryPointSize <= sizeof(int));

	if (0 == writeCount)
	{
		assert(out_pFailureEntryPointAdress != NULL);
		// Note that this line depends on little-endian architecture
		memcpy(out_pFailureEntryPointAdress, &in_failureEntryPointValue, 
			in_failureEntryPointSize);
		return 1; // Failure
	}
	else
	{
		assert(1 == writeCount);
		return 0; // Success
	}
}
