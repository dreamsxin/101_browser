/*
* Copyright 2011-2012 Wolfgang Keller
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

#include "IO/MemoryByteStream_v2.h"
#include "MiniStdlib/minmax.h"
#include <assert.h>
#include <string.h> // for memcpy

void memoryByteStream_v2ReadStateInit(
	MemoryByteStream_v2ReadState *in_pMemoryByteStream_v2ReadState,
	const void *in_pBuffer, size_t in_bufferSize)
{
	in_pMemoryByteStream_v2ReadState->buffer = in_pBuffer;
	in_pMemoryByteStream_v2ReadState->bufferSize = in_bufferSize;
	in_pMemoryByteStream_v2ReadState->bufferPos = 0;
}

void memoryByteStream_v2WriteStateInit(
	MemoryByteStream_v2WriteState *in_pMemoryByteStream_v2WriteState,
	void *in_pBuffer, size_t in_bufferSize)
{
	in_pMemoryByteStream_v2WriteState->buffer = in_pBuffer;
	in_pMemoryByteStream_v2WriteState->bufferSize = in_bufferSize;
	in_pMemoryByteStream_v2WriteState->bufferPos = 0;
}

void memoryByteStream_v2ReadStateReset(MemoryByteStream_v2ReadState *in_pMemoryByteStream_v2ReadState)
{
	in_pMemoryByteStream_v2ReadState->bufferPos = 0;
}

void memoryByteStream_v2WriteStateReset(MemoryByteStream_v2WriteState *in_pMemoryByteStream_v2WriteState)
{
	in_pMemoryByteStream_v2WriteState->bufferPos = 0;
}

void memoryByteReadStream_v2Read(void *in_out_pMemoryByteStream_v2ReadState, 
	void *out_pBuffer, size_t in_count, size_t *out_pCount, bool *out_pIsTerminal)
{
	MemoryByteStream_v2ReadState *pMemoryByteStream_v2ReadState = 
		(MemoryByteStream_v2ReadState*) in_out_pMemoryByteStream_v2ReadState;

	in_count = MIN(in_count, pMemoryByteStream_v2ReadState->bufferSize - pMemoryByteStream_v2ReadState->bufferPos);
	memcpy(out_pBuffer, (uint8_t*) pMemoryByteStream_v2ReadState->buffer+pMemoryByteStream_v2ReadState->bufferPos, in_count);
	pMemoryByteStream_v2ReadState->bufferPos += in_count;

	assert(pMemoryByteStream_v2ReadState->bufferPos <= pMemoryByteStream_v2ReadState->bufferSize);

	*out_pCount = in_count;
	*out_pIsTerminal = (pMemoryByteStream_v2ReadState->bufferSize == 
		pMemoryByteStream_v2ReadState->bufferPos);
}

void memoryByteReadStream_v2Write(void *in_out_pMemoryByteStream_v2ReadState, 
	const void *out_pBuffer, size_t in_count, size_t *out_pCount, bool in_isTerminal)
{
	MemoryByteStream_v2WriteState *pMemoryByteStream_v2WriteState = 
		(MemoryByteStream_v2WriteState*) in_out_pMemoryByteStream_v2ReadState;

	in_count = MIN(in_count, pMemoryByteStream_v2WriteState->bufferSize - pMemoryByteStream_v2WriteState->bufferPos);
	memcpy((uint8_t*) pMemoryByteStream_v2WriteState->buffer+pMemoryByteStream_v2WriteState->bufferPos, out_pBuffer, in_count);
	pMemoryByteStream_v2WriteState->bufferPos += in_count;

	assert(pMemoryByteStream_v2WriteState->bufferPos <= pMemoryByteStream_v2WriteState->bufferSize);

	*out_pCount = in_count;
}

ByteStreamInterface_v2 getMemoryByteStreamReadInterface_v2()
{
	ByteStreamInterface_v2 out_byteStreamInterface;
	memset(&out_byteStreamInterface, 0, sizeof(out_byteStreamInterface));
	out_byteStreamInterface.mpfRead = memoryByteReadStream_v2Read;
	return out_byteStreamInterface;
}

ByteStreamInterface_v2 getMemoryByteStreamWriteInterface_v2()
{
	ByteStreamInterface_v2 out_byteStreamInterface;
	memset(&out_byteStreamInterface, 0, sizeof(out_byteStreamInterface));
	out_byteStreamInterface.mpfWrite = memoryByteReadStream_v2Write;
	return out_byteStreamInterface;
}
