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

#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstdbool.h"

typedef enum
{
	PipeStreamStateTypeReader,
	PipeStreamStateTypeWriter
} PipeStreamStateType;

typedef struct
{
	uint8_t * volatile mpCurrentBuffer;
	uint8_t * volatile mpNextBuffer;
	volatile size_t mCurrentBufferSize;
	volatile size_t mNextBufferSize;
	PipeStreamStateType mCurrentStateType;
	CoroutineDescriptor *mpWriterDescriptor;
	CoroutineDescriptor *mpReaderDescriptor;
} PipeStreamState;

bool initPipeStreamState(PipeStreamState *in_pPipeStreamState, 
	bool in_isCurrentStreamWriter, CoroutineDescriptor *in_pOtherCoroutine,
	void *in_pUserData);

#endif
