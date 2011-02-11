/*
 * Copyright 2008-2011 Wolfgang Keller
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

#ifndef _MTAx_IO_PipeStream_h
#define _MTAx_IO_PipeStream_h

#include "Coroutine/Coroutine.h"
#include "IO/ByteStreamInterface.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/climits.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	PipeStreamStateTypeReader,
	PipeStreamStateTypeWriter
} PipeStreamStateType;

typedef struct _PipeStreamState
{
	const uint8_t * volatile mpCurrentBuffer;
	volatile size_t mCurrentBufferSize;
	PipeStreamStateType mCurrentStateType;
	CoroutineDescriptor *mpWriterDescriptor;
	CoroutineDescriptor *mpReaderDescriptor;

	// Data to pass to the kickoff routine
	void (*mpStartup)(struct _PipeStreamState*, void*);
	void *mpUserData;
} PipeStreamState;

/*!
 * Parameters:
 * out_pPipeStreamState: adress where the created PipeStreamState will be stored
 * in_isCurrentStreamWriter: set to true, if the current coroutine is the
 *                           writer coroutine;
 *                           set to false,  if the current coroutine is the
 *                           reader coroutine
 * in_pThisCoroutine: a pointer to the coroutine descriptor of the current coroutine
 * in_pOtherCoroutine: TODO
 * in_pUserData: user data to be sent to the new created coroutine
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
bool initPipeStreamState(PipeStreamState *out_pPipeStreamState,
	bool in_isCurrentStreamWriter,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(PipeStreamState*, void*),
	void *in_pUserData);

#ifdef _WIN32
__declspec(dllexport)
#endif
void deletePipeStreamState(PipeStreamState *out_pPipeStreamState);

#ifdef _WIN32
__declspec(dllexport)
#endif
size_t pipeStreamRead(void *in_out_pPipeStreamState, void *out_pBuffer, size_t in_count);

#ifdef _WIN32
__declspec(dllexport)
#endif
size_t pipeStreamWrite(void *in_out_pPipeStreamState, const void *in_pBuffer, size_t in_count);

const ByteStreamReadInterface cPipeStreamReadInterface = { &pipeStreamRead };
const ByteStreamWriteInterface cPipeStreamWriteInterface = { &pipeStreamWrite };

#ifdef __cplusplus
}
#endif

#endif
