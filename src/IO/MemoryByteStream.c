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

#include "IO/MemoryByteStream.h"
#include "MiniStdlib/minmax.h"
#include <assert.h>
#include <string.h> // for memcpy


void memoryByteStreamReadStateInit(
	MemoryByteStreamReadState *in_pMemoryByteStreamReadState,
	const void *in_pBuffer, size_t in_bufferSize)
{
	in_pMemoryByteStreamReadState->buffer = (const uint8_t*) in_pBuffer;
	in_pMemoryByteStreamReadState->bufferSize = in_bufferSize;
	in_pMemoryByteStreamReadState->bufferPos = 0;
}

size_t memoryByteReadStreamRead(void *in_out_pMemoryByteStreamReadState, 
	void *out_pBuffer, size_t in_count)
{
	MemoryByteStreamReadState *pMemoryByteStreamReadState = 
		(MemoryByteStreamReadState *) in_out_pMemoryByteStreamReadState;

	in_count = MIN(in_count, pMemoryByteStreamReadState->bufferSize - pMemoryByteStreamReadState->bufferPos);
	memcpy(out_pBuffer, pMemoryByteStreamReadState->buffer+pMemoryByteStreamReadState->bufferPos, in_count);
	pMemoryByteStreamReadState->bufferPos += in_count;

	assert(pMemoryByteStreamReadState->bufferPos <= pMemoryByteStreamReadState->bufferSize);

	return in_count;
}

ByteStreamInterface getMemoryByteStreamReadInterface()
{
	ByteStreamInterface out_byteStreamInterface;
	memset(&out_byteStreamInterface, 0, sizeof(out_byteStreamInterface));
	out_byteStreamInterface.mpfRead = memoryByteReadStreamRead;
	return out_byteStreamInterface;
}
