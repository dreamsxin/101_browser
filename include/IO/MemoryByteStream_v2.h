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

#ifndef _MTAx_IO_MemoryByteStream_v2_h
#define _MTAx_IO_MemoryByteStream_v2_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/declspec.h"
#include "IO/ByteStream_v2.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	const void *buffer;
	size_t bufferSize;
	size_t bufferPos;
} MemoryByteStream_v2ReadState;

typedef struct
{
	void *buffer;
	size_t bufferSize;
	size_t bufferPos;
} MemoryByteStream_v2WriteState;

DLLEXPORT void memoryByteStream_v2ReadStateInit(
	MemoryByteStream_v2ReadState *in_pMemoryByteStream_v2ReadState,
	const void *in_pBuffer, size_t in_bufferSize);
DLLEXPORT void memoryByteStream_v2WriteStateInit(
	MemoryByteStream_v2WriteState *in_pMemoryByteStream_v2WriteState,
	void *in_pBuffer, size_t in_bufferSize);

DLLEXPORT void memoryByteStream_v2ReadStateReset(MemoryByteStream_v2ReadState *in_pMemoryByteStream_v2ReadState);
DLLEXPORT void memoryByteStream_v2WriteStateReset(MemoryByteStream_v2WriteState *in_pMemoryByteStream_v2WriteState);

DLLEXPORT void memoryByteReadStream_v2Read(void *in_out_pMemoryByteStream_v2ReadState, 
	void *out_pBuffer, size_t in_count, size_t *out_pCount, bool *out_pIsTerminal);
DLLEXPORT void memoryByteReadStream_v2Write(void *in_out_pMemoryByteStream_v2ReadState, 
	const void *out_pBuffer, size_t in_count, size_t *out_pCount, bool in_isTerminal);

DLLEXPORT ByteStreamInterface_v2 getMemoryByteStreamReadInterface_v2();
DLLEXPORT ByteStreamInterface_v2 getMemoryByteStreamWriteInterface_v2();

#ifdef __cplusplus
}
#endif

#endif