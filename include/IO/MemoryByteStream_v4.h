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

#ifndef _MTAx_IO_MemoryByteStream_v4_h
#define _MTAx_IO_MemoryByteStream_v4_h

#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"
#include "IO/ByteStream_v4.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	// Permanent data
	union {
		const void *readBuffer;
		void *writeBuffer;
	} rwBuffer;
	size_t blockSize;
	size_t bufferBlockCount;
	bool triggerAsEarlyAsPossible;
	bool terminateAfterLastOperation;

	// Temporary data
	size_t currentBufferBlockIndex;
	ByteStreamStatus_v4 status;
} MemoryByteStream_v4State;

DLLEXPORT void memoryByteStream_v4ReadStateInit(
	MemoryByteStream_v4State *out_pMemoryByteStream_v4ReadState,
	const void *in_pBuffer, size_t in_blockSize, size_t in_bufferBlockCount, 
	bool in_triggerAsEarlyAsPossible, 
	bool in_terminateAfterLastOperation);
DLLEXPORT void memoryByteStream_v4WriteStateInit(
	MemoryByteStream_v4State *out_pMemoryByteStream_v4WriteState,
	void *in_pBuffer, size_t in_blockSize, size_t in_bufferBlockCount, 
	bool in_triggerAsEarlyAsPossible);

DLLEXPORT void memoryByteStream_v4StateReset(MemoryByteStream_v4State *in_pMemoryByteStream_v4State);

size_t memoryByteStream_v4Read(void *in_out_pByteStreamState, 
	void *out_pBuffer, size_t in_count);
size_t memoryByteStream_v4Write(void *in_out_pByteStreamState, 
	const void *in_pBuffer, size_t in_count);

ByteStreamStatus_v4 memoryByteStream_v4GetStatus(const void *in_pByteStreamState);
void memoryByteStream_v4SetStatus(void *in_out_pByteStreamState, ByteStreamStatus_v4 in_status);

size_t memoryByteStream_v4Read(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count);
size_t memoryByteStream_v4Write(void *in_out_pByteStreamState, 
		const void *in_pBuffer, size_t in_count);

DLLEXPORT ByteStreamReadInterface_v4 memoryByteStreamReadInterface_v4_get();
DLLEXPORT ByteStreamWriteInterface_v4 memoryByteStreamWriteInterface_v4_get();

#ifdef __cplusplus
}
#endif

#endif
