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

#ifndef _MTAx_IO_MemoryByteStream_v3_h
#define _MTAx_IO_MemoryByteStream_v3_h

#include "MiniStdlib/declspec.h"
#include "IO/ByteStream_v3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	union {
		const void *readBuffer;
		void *writeBuffer;
	} rwBuffer;
	size_t bufferSize;
	size_t bufferPos;
	bool terminateAfterLastOperation;
	bool isTerminated;
} MemoryByteStream_v3State;

DLLEXPORT void memoryByteStream_v3ReadStateInit(
	MemoryByteStream_v3State *out_pMemoryByteStream_v3ReadState,
	const void *in_pBuffer, size_t in_bufferSize, 
	bool in_terminateAfterLastOperation);
DLLEXPORT void memoryByteStream_v3WriteStateInit(
	MemoryByteStream_v3State *out_pMemoryByteStream_v3WriteState,
	void *in_pBuffer, size_t in_bufferSize, 
	bool in_terminateAfterLastOperation);

DLLEXPORT void memoryByteStream_v3StateReset(MemoryByteStream_v3State *in_pMemoryByteStream_v3State);

size_t memoryByteStream_v3Read(void *in_out_pByteStreamState, 
	void *out_pBuffer, size_t in_count);
size_t memoryByteStream_v3Write(void *in_out_pByteStreamState, 
	const void *in_pBuffer, size_t in_count);

bool memoryByteStream_v3IsTerminated(const void *in_pByteStreamState);
void memoryByteStream_v3Terminate(void *out_pByteStreamState);

size_t memoryByteStream_v3Read(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count);
size_t memoryByteStream_v3Write(void *in_out_pByteStreamState, 
		const void *in_pBuffer, size_t in_count);

DLLEXPORT ByteStreamReadInterface_v3 memoryByteStreamReadInterface_v3_get();
DLLEXPORT ByteStreamWriteInterface_v3 memoryByteStreamWriteInterface_v3_get();

#ifdef __cplusplus
}
#endif

#endif
