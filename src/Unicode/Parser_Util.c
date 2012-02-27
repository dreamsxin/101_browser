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
#include <string.h> // memset
#include <assert.h>

const UnicodeCodePoint cReplacementCharacter = 0xFFFD;

bool utfX_isTerminated(const void *in_pByteStreamState)
{
	assert(((ParserState*) in_pByteStreamState)->readInterface.
		commonByteStreamInterface.mpfIsTerminated != NULL);

	return ((ParserState *) in_pByteStreamState)->readInterface.
		commonByteStreamInterface.mpfIsTerminated(
		((ParserState *) in_pByteStreamState)->pReadState);
}

void utfX_Terminate(void *in_out_pByteStreamState)
{
	assert(((ParserState *) in_out_pByteStreamState)->readInterface.
		commonByteStreamInterface.mpfTerminate != NULL);

	((ParserState *) in_out_pByteStreamState)->readInterface.
		commonByteStreamInterface.mpfTerminate(
		((ParserState *) in_out_pByteStreamState)->pReadState);
}

ByteStreamReadInterface_v3 getParser_ReadInterface(
	size_t (*mpfReadFunction)(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count))
{
	ByteStreamReadInterface_v3 out_interface;
	memset(&out_interface, 0, sizeof(out_interface));

	out_interface.commonByteStreamInterface.mpfIsTerminated = utfX_isTerminated;
	out_interface.commonByteStreamInterface.mpfTerminate = utfX_Terminate;
	out_interface.mpfRead = mpfReadFunction;

	return out_interface;
}

void writeCodePoint(UnicodeCodePoint **out_ppBuffer, 
	size_t *in_out_pWriteCount, UnicodeCodePoint in_codePoint)
{
	**out_ppBuffer = in_codePoint;
	(*in_out_pWriteCount)++;
	(*out_ppBuffer)++;
}