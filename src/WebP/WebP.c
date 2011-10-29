/*
* Copyright 2011 Wolfgang Keller
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

#include "WebP/WebP.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstring.h"
#include "IO/SetjmpStream.h"

// See http://msdn.microsoft.com/en-us/library/ms713231
typedef struct
{
	char ID[4];
	uint32_t size;
	char FormType[4];
} RIFF_Header;

ReadResult readWebP(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface)
{
	SetjmpStreamState setjmpReadStreamState;
	jmp_buf jmpBuf;
	ByteStreamInterface setjmpReadStreamInterface;
	int result;

	RIFF_Header riffHeader;

	setjmpStreamInit(&setjmpReadStreamState, &jmpBuf, ReadResultPrematureEndOfStream, 
		in_out_pReadStreamState, in_readInterface);
	setjmpReadStreamInterface = getSetjmpStreamByteStreamInterface(&setjmpReadStreamState);
	
	if ((result = setjmp(jmpBuf)) != 0)
		return (ReadResult) result;
	
	setjmpReadStreamInterface.mpfRead(&setjmpReadStreamState, &riffHeader, sizeof(riffHeader));

	if (memcmp(riffHeader.ID, "RIFF", 4))
		longjmp(jmpBuf, ReadResultInvalidData);

	// 4: the 4 remaining bytes of the header
	if (riffHeader.size < 4)
		longjmp(jmpBuf, ReadResultInvalidData);

	riffHeader.size -= 4;


}
