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

#include "IO/MemoryByteStream_v4.h"
#include "MiniStdlib/minmax.h"
#include <assert.h>
#include <string.h> // for memcpy
#include <stdint.h>

void memoryByteStream_v4StateInit(
	MemoryByteStream_v4State *out_pMemoryByteStream_v4State,
	size_t in_blockSize, size_t in_bufferBlockCount, 
	bool in_triggerAsEarlyAsPossible, 
	bool in_terminateAfterLastOperation)
{
	out_pMemoryByteStream_v4State->blockSize = in_blockSize;
	out_pMemoryByteStream_v4State->bufferBlockCount = in_bufferBlockCount;
	out_pMemoryByteStream_v4State->triggerAsEarlyAsPossible = in_triggerAsEarlyAsPossible;
	out_pMemoryByteStream_v4State->terminateAfterLastOperation = in_terminateAfterLastOperation;

	memoryByteStream_v4StateReset(out_pMemoryByteStream_v4State);
}

void memoryByteStream_v4ReadStateInit(
	MemoryByteStream_v4State *out_pMemoryByteStream_v4ReadState,
	const void *in_pBuffer, size_t in_blockSize, size_t in_bufferBlockCount, 
	bool in_triggerAsEarlyAsPossible, 
	bool in_terminateAfterLastOperation)
{
	memoryByteStream_v4StateInit(out_pMemoryByteStream_v4ReadState, 
		in_blockSize, in_bufferBlockCount, in_triggerAsEarlyAsPossible, 
		in_terminateAfterLastOperation);

	out_pMemoryByteStream_v4ReadState->rwBuffer.readBuffer = in_pBuffer;
}

void memoryByteStream_v4WriteStateInit(
	MemoryByteStream_v4State *out_pMemoryByteStream_v4WriteState,
	void *in_pBuffer, size_t in_blockSize, size_t in_bufferBlockCount, 
	bool in_triggerAsEarlyAsPossible, 
	bool in_terminateAfterLastOperation)
{
	memoryByteStream_v4StateInit(out_pMemoryByteStream_v4WriteState, 
		in_blockSize, in_bufferBlockCount, in_triggerAsEarlyAsPossible, 
		in_terminateAfterLastOperation);
	
	out_pMemoryByteStream_v4WriteState->rwBuffer.writeBuffer = in_pBuffer;
}

static bool memoryByteStream_v4GetTriggerAsEarlyAsPossible(const MemoryByteStream_v4State *in_pByteStreamState)
{
	return in_pByteStreamState->triggerAsEarlyAsPossible;
}

ByteStreamStatus_v4 memoryByteStream_v4GetStatus(const void *in_pByteStreamState)
{
	const MemoryByteStream_v4State *cpState = (const MemoryByteStream_v4State *) 
		in_pByteStreamState;

	if (!cpState->isTriggered)
		return ByteStreamStatus_OK;
	else
	{
		if (!cpState->terminateAfterLastOperation)
			return ByteStreamStatus_InterventionRequired;
		else
			return ByteStreamStatus_Terminated;
	}
}

void memoryByteStream_v4Terminate(void *out_pByteStreamState)
{
	((MemoryByteStream_v4State *) out_pByteStreamState)->isTriggered = true;
	((MemoryByteStream_v4State *) out_pByteStreamState)->terminateAfterLastOperation;

	assert(ByteStreamStatus_Terminated == memoryByteStream_v4GetStatus(out_pByteStreamState));
}

void memoryByteStream_v4StateReset(MemoryByteStream_v4State *in_pMemoryByteStream_v4State)
{
	in_pMemoryByteStream_v4State->currentBufferBlockIndex = 0;
	in_pMemoryByteStream_v4State->isTriggered = (in_pMemoryByteStream_v4State->triggerAsEarlyAsPossible && 
		0 == in_pMemoryByteStream_v4State->bufferBlockCount);
}

size_t memoryByteStream_v4Copy(MemoryByteStream_v4State *in_out_pMemoryByteStream_v4State, 
	const void *in_pBuffer, void *out_pBuffer, size_t in_count)
{
	size_t copyCount;

	if (in_out_pMemoryByteStream_v4State->isTriggered)
		return 0;
	
	assert(in_out_pMemoryByteStream_v4State->currentBufferBlockIndex <= 
		in_out_pMemoryByteStream_v4State->bufferBlockCount);

	copyCount = MIN(in_count, in_out_pMemoryByteStream_v4State->bufferBlockCount 
		- in_out_pMemoryByteStream_v4State->currentBufferBlockIndex);
	memcpy(out_pBuffer, in_pBuffer, copyCount * in_out_pMemoryByteStream_v4State->blockSize);
	in_out_pMemoryByteStream_v4State->currentBufferBlockIndex += copyCount;

	assert(in_out_pMemoryByteStream_v4State->currentBufferBlockIndex <= in_out_pMemoryByteStream_v4State->bufferBlockCount);

	if (memoryByteStream_v4GetTriggerAsEarlyAsPossible(in_out_pMemoryByteStream_v4State))
	{
		in_out_pMemoryByteStream_v4State->isTriggered = 
			(in_out_pMemoryByteStream_v4State->bufferBlockCount == in_out_pMemoryByteStream_v4State->currentBufferBlockIndex);
	}
	else
	{
		assert(!memoryByteStream_v4GetTriggerAsEarlyAsPossible(in_out_pMemoryByteStream_v4State));
		in_out_pMemoryByteStream_v4State->isTriggered = (copyCount < in_count);
	}

	return copyCount;
}

size_t memoryByteStream_v4Read(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count)
{
	MemoryByteStream_v4State *pMemoryByteStream_v4State = 
		(MemoryByteStream_v4State*) in_out_pByteStreamState;

	return memoryByteStream_v4Copy(pMemoryByteStream_v4State, 
		(uint8_t*) pMemoryByteStream_v4State->rwBuffer.readBuffer
		+ pMemoryByteStream_v4State->currentBufferBlockIndex, out_pBuffer, in_count);
}

size_t memoryByteStream_v4Write(void *in_out_pByteStreamState, 
		const void *in_pBuffer, size_t in_count)
{
	MemoryByteStream_v4State *pMemoryByteStream_v4State = 
		(MemoryByteStream_v4State*) in_out_pByteStreamState;

	return memoryByteStream_v4Copy(pMemoryByteStream_v4State, 
		in_pBuffer, (uint8_t*) pMemoryByteStream_v4State->rwBuffer.writeBuffer
		+ pMemoryByteStream_v4State->currentBufferBlockIndex, in_count);
}

void initCommonInterface(CommonByteStreamInterface_v4 *out_pCommon)
{
	out_pCommon->mpfGetStatus = memoryByteStream_v4GetStatus;
	out_pCommon->mpfTerminate = memoryByteStream_v4Terminate;
}

ByteStreamReadInterface_v4 memoryByteStreamReadInterface_v4_get()
{
	ByteStreamReadInterface_v4 out_interface;
	memset(&out_interface, 0, sizeof(out_interface));
	initCommonInterface(&out_interface.commonByteStreamInterface);
	out_interface.mpfRead = memoryByteStream_v4Read;

	return out_interface;
}

ByteStreamWriteInterface_v4 memoryByteStreamWriteInterface_v4_get()
{
	ByteStreamWriteInterface_v4 out_interface;
	memset(&out_interface, 0, sizeof(out_interface));
	initCommonInterface(&out_interface.commonByteStreamInterface);
	out_interface.mpfWrite = memoryByteStream_v4Write;

	return out_interface;
}
