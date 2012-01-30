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

void memoryByteStream_v2StateInit(
	MemoryByteStream_v2State *in_pMemoryByteStream_v2State,
	size_t in_bufferSize)
{
	in_pMemoryByteStream_v2State->bufferSize = in_bufferSize;
	in_pMemoryByteStream_v2State->bufferPos = 0;
	in_pMemoryByteStream_v2State->isTerminated = false;
}

void memoryByteStream_v2ReadStateInit(
	MemoryByteStream_v2State *in_pMemoryByteStream_v2ReadState,
	const void *in_pBuffer, size_t in_bufferSize)
{
	in_pMemoryByteStream_v2ReadState->rwBuffer.readBuffer = in_pBuffer;
	memoryByteStream_v2StateInit(in_pMemoryByteStream_v2ReadState, in_bufferSize);
}

void memoryByteStream_v2WriteStateInit(
	MemoryByteStream_v2State *in_pMemoryByteStream_v2WriteState,
	void *in_pBuffer, size_t in_bufferSize)
{
	in_pMemoryByteStream_v2WriteState->rwBuffer.writeBuffer = in_pBuffer;
	memoryByteStream_v2StateInit(in_pMemoryByteStream_v2WriteState, in_bufferSize);
}

void memoryByteStream_v2StateReset(MemoryByteStream_v2State *in_pMemoryByteStream_v2State)
{
	in_pMemoryByteStream_v2State->bufferPos = 0;
	in_pMemoryByteStream_v2State->isTerminated = false;
}

void memoryByteStream_v2Copy(MemoryByteStream_v2State *in_out_pMemoryByteStream_v2State, 
	const void *in_pBuffer, void *out_pBuffer, size_t in_count, bool in_isTerminal, 
		size_t *out_pCount, bool *out_pIsTerminal)
{
	assert(!in_out_pMemoryByteStream_v2State->isTerminated);
	assert(in_out_pMemoryByteStream_v2State->bufferPos <= in_out_pMemoryByteStream_v2State->bufferSize);

	in_count = MIN(in_count, in_out_pMemoryByteStream_v2State->bufferSize 
		- in_out_pMemoryByteStream_v2State->bufferPos);
	memcpy(out_pBuffer, in_pBuffer, in_count);
	in_out_pMemoryByteStream_v2State->bufferPos += in_count;

	assert(in_out_pMemoryByteStream_v2State->bufferPos <= in_out_pMemoryByteStream_v2State->bufferSize);

	assert(out_pCount != NULL);
	*out_pCount = in_count;

	if (out_pIsTerminal)
		*out_pIsTerminal = (in_out_pMemoryByteStream_v2State->bufferSize == in_out_pMemoryByteStream_v2State->bufferPos);

	if (in_isTerminal && in_count, in_out_pMemoryByteStream_v2State->bufferSize == 
		in_count, in_out_pMemoryByteStream_v2State->bufferPos)
		in_out_pMemoryByteStream_v2State->isTerminated = true;
}

void memoryByteReadStream_v2Read(void *in_out_pMemoryByteStream_v2State, 
	void *out_pBuffer, size_t in_count, bool in_isTerminal, 
	size_t *out_pCount, bool *out_pIsTerminal)
{
	MemoryByteStream_v2State *pMemoryByteStream_v2State = 
		(MemoryByteStream_v2State*) in_out_pMemoryByteStream_v2State;

	memoryByteStream_v2Copy(pMemoryByteStream_v2State, 
		(uint8_t*) pMemoryByteStream_v2State->rwBuffer.readBuffer
		+ pMemoryByteStream_v2State->bufferPos, out_pBuffer, 
		in_count, in_isTerminal, out_pCount, out_pIsTerminal);
}

void memoryByteReadStream_v2Write(void *in_out_pMemoryByteStream_v2State, 
	const void *in_pBuffer, size_t in_count, bool in_isTerminal, 
	size_t *out_pCount, bool *out_pIsTerminal)
{
	MemoryByteStream_v2State *pMemoryByteStream_v2State = 
		(MemoryByteStream_v2State*) in_out_pMemoryByteStream_v2State;

	memoryByteStream_v2Copy(pMemoryByteStream_v2State, 
		in_pBuffer, (uint8_t*) pMemoryByteStream_v2State->rwBuffer.writeBuffer
		+ pMemoryByteStream_v2State->bufferPos, 
		in_count, in_isTerminal, out_pCount, out_pIsTerminal);
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
