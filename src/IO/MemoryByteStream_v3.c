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

#include "IO/MemoryByteStream_v3.h"
#include "MiniStdlib/minmax.h"
#include <assert.h>
#include <string.h> // for memcpy
#include <stdint.h>

void memoryByteStream_v3StateInit(
	MemoryByteStream_v3State *out_pMemoryByteStream_v3State,
	size_t in_bufferSize, bool in_terminateAfterLastOperation)
{
	out_pMemoryByteStream_v3State->bufferSize = in_bufferSize;
	out_pMemoryByteStream_v3State->terminateAfterLastOperation = in_terminateAfterLastOperation;

	memoryByteStream_v3StateReset(out_pMemoryByteStream_v3State);
}

void memoryByteStream_v3ReadStateInit(
	MemoryByteStream_v3State *out_pMemoryByteStream_v3ReadState,
	const void *in_pBuffer, size_t in_bufferSize, 
	bool in_terminateAfterLastOperation)
{
	memoryByteStream_v3StateInit(out_pMemoryByteStream_v3ReadState, 
		in_bufferSize, in_terminateAfterLastOperation);

	out_pMemoryByteStream_v3ReadState->rwBuffer.readBuffer = in_pBuffer;
}

void memoryByteStream_v3WriteStateInit(
	MemoryByteStream_v3State *out_pMemoryByteStream_v3WriteState,
	void *in_pBuffer, size_t in_bufferSize, 
	bool in_terminateAfterLastOperation)
{
	memoryByteStream_v3StateInit(out_pMemoryByteStream_v3WriteState, 
		in_bufferSize, in_terminateAfterLastOperation);
	
	out_pMemoryByteStream_v3WriteState->rwBuffer.writeBuffer = in_pBuffer;
}

bool memoryByteStream_v3GetTerminateAfterLastOperation(const void *in_pByteStreamState)
{
	return ((const MemoryByteStream_v3State *) in_pByteStreamState)->terminateAfterLastOperation;;
}

bool memoryByteStream_v3IsTerminated(const void *in_pByteStreamState)
{
	return ((const MemoryByteStream_v3State *) in_pByteStreamState)->isTerminated;
}

void memoryByteStream_v3Terminate(void *out_pByteStreamState)
{
	((MemoryByteStream_v3State *) out_pByteStreamState)->isTerminated = true;

	assert(memoryByteStream_v3IsTerminated(out_pByteStreamState));
}

void memoryByteStream_v3StateReset(MemoryByteStream_v3State *in_pMemoryByteStream_v3State)
{
	in_pMemoryByteStream_v3State->bufferPos = 0;
	in_pMemoryByteStream_v3State->isTerminated = (in_pMemoryByteStream_v3State->terminateAfterLastOperation && 
		0 == in_pMemoryByteStream_v3State->bufferSize);
}

size_t memoryByteStream_v3Copy(MemoryByteStream_v3State *in_out_pMemoryByteStream_v3State, 
	const void *in_pBuffer, void *out_pBuffer, size_t in_count)
{
	size_t copyCount;

	if (in_out_pMemoryByteStream_v3State->isTerminated)
		return 0;
	
	assert(in_out_pMemoryByteStream_v3State->bufferPos <= in_out_pMemoryByteStream_v3State->bufferSize);

	copyCount = MIN(in_count, in_out_pMemoryByteStream_v3State->bufferSize 
		- in_out_pMemoryByteStream_v3State->bufferPos);
	memcpy(out_pBuffer, in_pBuffer, copyCount);
	in_out_pMemoryByteStream_v3State->bufferPos += copyCount;

	assert(in_out_pMemoryByteStream_v3State->bufferPos <= in_out_pMemoryByteStream_v3State->bufferSize);

	if (memoryByteStream_v3GetTerminateAfterLastOperation(in_out_pMemoryByteStream_v3State))
	{
		in_out_pMemoryByteStream_v3State->isTerminated = 
			(in_out_pMemoryByteStream_v3State->bufferSize == in_out_pMemoryByteStream_v3State->bufferPos);
	}
	else
	{
		assert(!memoryByteStream_v3GetTerminateAfterLastOperation(in_out_pMemoryByteStream_v3State));
		in_out_pMemoryByteStream_v3State->isTerminated = (copyCount < in_count);
	}

	return copyCount;
}

size_t memoryByteStream_v3Read(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count)
{
	MemoryByteStream_v3State *pMemoryByteStream_v3State = 
		(MemoryByteStream_v3State*) in_out_pByteStreamState;

	return memoryByteStream_v3Copy(pMemoryByteStream_v3State, 
		(uint8_t*) pMemoryByteStream_v3State->rwBuffer.readBuffer
		+ pMemoryByteStream_v3State->bufferPos, out_pBuffer, in_count);
}

size_t memoryByteStream_v3Write(void *in_out_pByteStreamState, 
		const void *in_pBuffer, size_t in_count)
{
	MemoryByteStream_v3State *pMemoryByteStream_v3State = 
		(MemoryByteStream_v3State*) in_out_pByteStreamState;

	return memoryByteStream_v3Copy(pMemoryByteStream_v3State, 
		in_pBuffer, (uint8_t*) pMemoryByteStream_v3State->rwBuffer.writeBuffer
		+ pMemoryByteStream_v3State->bufferPos, in_count);
}

void initCommonInterface(CommonByteStreamInterface_v3 *out_pCommon)
{
	out_pCommon->mpfGetTerminateAfterLastOperation = memoryByteStream_v3GetTerminateAfterLastOperation;
	out_pCommon->mpfIsTerminated = memoryByteStream_v3IsTerminated;
	out_pCommon->mpfTerminate = memoryByteStream_v3Terminate;
}

ByteStreamReadInterface_v3 memoryByteStreamReadInterface_v3_get()
{
	ByteStreamReadInterface_v3 out_interface;
	memset(&out_interface, 0, sizeof(out_interface));
	initCommonInterface(&out_interface.commonByteStreamInterface);
	out_interface.mpfRead = memoryByteStream_v3Read;

	return out_interface;
}

ByteStreamWriteInterface_v3 memoryByteStreamWriteInterface_v3_get()
{
	ByteStreamWriteInterface_v3 out_interface;
	memset(&out_interface, 0, sizeof(out_interface));
	initCommonInterface(&out_interface.commonByteStreamInterface);
	out_interface.mpfWrite = memoryByteStream_v3Write;

	return out_interface;
}
