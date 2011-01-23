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

#include "IO/PipeStream.h"

typedef struct
{
	void (*mpStartup)(PipeStreamState*, void*);
	PipeStreamState *mpState;
	void *mpUserData;
} PipeStreamData;

void
#ifdef _WIN32
__stdcall
#endif	
pipeStreamKickoff(void *in_pPipeStreamData)
{
	PipeStreamData *pData = (PipeStreamData*) in_pPipeStreamData;

	(*pData->mpStartup)(pData->mpState, pData->mpUserData);
}

bool initPipeStreamState(PipeStreamState *out_pPipeStreamState,
	bool in_isCurrentStreamWriter,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(PipeStreamState*, void*),
	void *in_pUserData)
{
	PipeStreamData pipeStreamData = { in_pOtherCoroutineStartup, out_pPipeStreamState, in_pUserData };

	// Not necessary, but more safe...
	out_pPipeStreamState->mpCurrentBuffer = NULL;
	out_pPipeStreamState->mpNextBuffer = NULL;

	out_pPipeStreamState->mCurrentBufferSize = 0;
	out_pPipeStreamState->mNextBufferSize = 0;

	if (in_isCurrentStreamWriter)
	{
		out_pPipeStreamState->mCurrentStateType = PipeStreamStateTypeWriter;
		out_pPipeStreamState->mpWriterDescriptor = out_pThisCoroutine;
		out_pPipeStreamState->mpReaderDescriptor = out_pOtherCoroutine;
	}
	else
	{
		out_pPipeStreamState->mCurrentStateType = PipeStreamStateTypeReader;
		out_pPipeStreamState->mpReaderDescriptor = out_pThisCoroutine;
		out_pPipeStreamState->mpWriterDescriptor = out_pOtherCoroutine;
	}

	if (!convertThreadToCoroutine(out_pThisCoroutine))
		return false;

	if (!createCoroutine(0, &pipeStreamKickoff, &pipeStreamData, out_pOtherCoroutine))
		return false;

	return true;
}
