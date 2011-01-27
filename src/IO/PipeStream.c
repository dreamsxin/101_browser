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

#include "MiniStdlib/minmax.h"
#include "IO/PipeStream.h"
#include <assert.h>

void
#ifdef _WIN32
__stdcall
#endif	
pipeStreamKickoff(void *in_pPipeStreamData)
{
	PipeStreamState *pState = (PipeStreamState*) in_pPipeStreamData;

	(*pState->mpStartup)(pState, pState->mpUserData);

	while (1)
	{
		size_t bytesCount;

		if (pState->mCurrentStateType == PipeStreamStateTypeReader)
		{
			bytesCount = pipeStreamRead(pState, NULL, 0);
		}
		else
		{
			bytesCount = pipeStreamWrite(pState, NULL, 0);
		}

		assert(0 == bytesCount);
	}
}

bool initPipeStreamState(PipeStreamState *out_pPipeStreamState,
	bool in_isCurrentStreamWriter,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(PipeStreamState*, void*),
	void *in_pUserData)
{
	// Not necessary, but more safe...
	out_pPipeStreamState->mpCurrentBuffer = NULL;

	out_pPipeStreamState->mCurrentBufferSize = 0;

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

	out_pPipeStreamState->mpStartup = in_pOtherCoroutineStartup;
	out_pPipeStreamState->mpUserData = in_pUserData;

	if (!convertThreadToCoroutine(out_pThisCoroutine))
		return false;

	if (!createCoroutine(0, &pipeStreamKickoff, out_pPipeStreamState, out_pOtherCoroutine))
		return false;

	return true;
}

void deletePipeStreamState(PipeStreamState *out_pPipeStreamState)
{
	if (PipeStreamStateTypeReader == out_pPipeStreamState->mCurrentStateType)
	{
		deleteCoroutine(out_pPipeStreamState->mpWriterDescriptor);
		out_pPipeStreamState->mpReaderDescriptor = NULL;
	}
	else
	{
		deleteCoroutine(out_pPipeStreamState->mpReaderDescriptor);
		out_pPipeStreamState->mpWriterDescriptor = NULL;
	}

	out_pPipeStreamState->mpCurrentBuffer = NULL;
	out_pPipeStreamState->mCurrentBufferSize = 0;
}

size_t pipeStreamRead(void *in_out_pPipeStreamState, void *out_pBuffer, size_t in_count)
{
	PipeStreamState *pPipeStreamState = (PipeStreamState*) in_out_pPipeStreamState;
	uint8_t *pBuffer = (uint8_t*) out_pBuffer;
	size_t bytesRead = 0;

	assert(PipeStreamStateTypeReader == pPipeStreamState->mCurrentStateType);

	// This means we want to terminate...
	if (0 == in_count)
	{
		pPipeStreamState->mCurrentStateType = PipeStreamStateTypeWriter;
		switchToCoroutine(pPipeStreamState->mpReaderDescriptor, pPipeStreamState->mpWriterDescriptor);
		assert(PipeStreamStateTypeReader == pPipeStreamState->mCurrentStateType);

		return 0;
	}

	while (bytesRead < in_count)
	{
		size_t bytesToReadInCurrentIteration = MIN(in_count-bytesRead, pPipeStreamState->mCurrentBufferSize);
		
		if (0 == bytesToReadInCurrentIteration)
		{
			pPipeStreamState->mCurrentStateType = PipeStreamStateTypeWriter;
			switchToCoroutine(pPipeStreamState->mpReaderDescriptor, pPipeStreamState->mpWriterDescriptor);
			assert(PipeStreamStateTypeReader == pPipeStreamState->mCurrentStateType);
			
			// that means the stream is terminated
			if (0 == pPipeStreamState->mCurrentBufferSize)
				return bytesRead;
		}
		else
		{
			memcpy(pBuffer, pPipeStreamState->mpCurrentBuffer, bytesToReadInCurrentIteration);

			bytesRead += bytesToReadInCurrentIteration;
			pBuffer += bytesToReadInCurrentIteration;
			pPipeStreamState->mpCurrentBuffer += bytesToReadInCurrentIteration;
			pPipeStreamState->mCurrentBufferSize -= bytesToReadInCurrentIteration;
		}
	}

	return bytesRead;
}

size_t pipeStreamWrite(void *in_out_pPipeStreamState, const void *in_pBuffer, size_t in_count)
{
	PipeStreamState *pPipeStreamState = (PipeStreamState*) in_out_pPipeStreamState;

	assert(PipeStreamStateTypeWriter == pPipeStreamState->mCurrentStateType);

	/*
	 * Note that pPipeStreamState->mCurrentBufferSize becomes != 0
	 * exactly in the case of calling pipeStreamRead([...], [...], 0)
	 * when pPipeStreamState->mCurrentBufferSize != 0.
	 */

	pPipeStreamState->mCurrentBufferSize = in_count;
	pPipeStreamState->mpCurrentBuffer = (const uint8_t*) in_pBuffer;

	pPipeStreamState->mCurrentStateType = PipeStreamStateTypeReader;
	switchToCoroutine(pPipeStreamState->mpWriterDescriptor, pPipeStreamState->mpReaderDescriptor);
	assert(PipeStreamStateTypeWriter == pPipeStreamState->mCurrentStateType);

	return in_count - pPipeStreamState->mCurrentBufferSize;
}
