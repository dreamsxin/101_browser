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

#ifndef _MTAx_IO_MemoryByteStream_h
#define _MTAx_IO_MemoryByteStream_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/declspec.h"
#include "IO/ByteStreamInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	const uint8_t *buffer;
	size_t bufferSize;
	size_t bufferPos;
} MemoryByteStreamReadState;

DLLEXPORT void initMemoryByteStreamReadState(
	MemoryByteStreamReadState *in_pMemoryByteStreamReadState,
	const void *in_pBuffer, size_t in_bufferSize);

DLLEXPORT size_t memoryByteReadStreamRead(void *in_out_pMemoryByteStreamReadState, 
	void *out_pBuffer, size_t in_count);

// TODO: implement pIsTerminated
const ByteStreamReadInterface cMemoryStreamReadInterface = { &memoryByteReadStreamRead, NULL };

#ifdef __cplusplus
}
#endif

#endif
