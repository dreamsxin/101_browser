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
#include "MiniStdlib/xchg.h"
#include "IO/PipeStream.h"
#include <assert.h>
#include <string.h> // for memcpy


void
#ifdef _WIN32
__stdcall
#endif
pipeStreamKickoffReader(void *in_pPipeStreamData)
{
	PipeStreamStateAndKickoff stateAndKickoff = *(PipeStreamStateAndKickoff*) in_pPipeStreamData;

	/*
	* No exchanging of descriptors since it hasn't been done originally when switching to
	* this coroutine.
	*/
	switchToCoroutine(stateAndKickoff.pState->mpOtherCoroutineDescriptor, stateAndKickoff.pState->mpCurrentCoroutineDescriptor);

	(*stateAndKickoff.kickoffData.mpStartup)(
		stateAndKickoff.pState, 
		stateAndKickoff.kickoffData.mpUserData);

	while (1)
	{
		size_t bytesCount = pipeStreamRead(stateAndKickoff.pState, NULL, 0);
		assert(0 == bytesCount);
	}
}

void
#ifdef _WIN32
__stdcall
#endif
pipeStreamKickoffWriter(void *in_pPipeStreamData)
{
	PipeStreamStateAndKickoff stateAndKickoff = *(PipeStreamStateAndKickoff*) in_pPipeStreamData;

	/*
	* No exchanging of descriptors since it hasn't been done originally when switching to
	* this coroutine.
	*/
	switchToCoroutine(stateAndKickoff.pState->mpOtherCoroutineDescriptor, stateAndKickoff.pState->mpCurrentCoroutineDescriptor);

	(*stateAndKickoff.kickoffData.mpStartup)(
		stateAndKickoff.pState, 
		stateAndKickoff.kickoffData.mpUserData);

	while (1)
	{
		size_t bytesCount = pipeStreamWrite(stateAndKickoff.pState, NULL, 0);
		assert(0 == bytesCount);
	}
}

bool initPipeStreamState(PipeStreamState *out_pPipeStreamState,
	bool in_isOtherStreamReader,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(PipeStreamState*, void*),
	void *in_pUserData)
{
	PipeStreamStateAndKickoff stateAndKickoff;

	stateAndKickoff.pState = out_pPipeStreamState;

	// Not necessary, but more safe...
	out_pPipeStreamState->mpCurrentBuffer = NULL;
	out_pPipeStreamState->mCurrentBufferSize = 0;

	out_pPipeStreamState->mpCurrentCoroutineDescriptor = out_pThisCoroutine;
	out_pPipeStreamState->mpOtherCoroutineDescriptor = out_pOtherCoroutine;

	stateAndKickoff.kickoffData.mpStartup = in_pOtherCoroutineStartup;
	stateAndKickoff.kickoffData.mpUserData = in_pUserData;

	if (!convertThreadToCoroutine(out_pThisCoroutine))
		return false;

	if (!createCoroutine(0, 
		in_isOtherStreamReader ? pipeStreamKickoffReader : pipeStreamKickoffWriter, 
		&stateAndKickoff, out_pOtherCoroutine))
		return false;

	/*
	* No exchanging of coroutine descriptors since we will immediately
	* come back (and the kickoff function knows of this property).
	*/
	switchToCoroutine(out_pThisCoroutine, out_pOtherCoroutine);

	return true;
}

void deletePipeStreamState(PipeStreamState *out_pPipeStreamState)
{
	deleteCoroutine(out_pPipeStreamState->mpOtherCoroutineDescriptor);
	out_pPipeStreamState->mpOtherCoroutineDescriptor = NULL;
	out_pPipeStreamState->mpCurrentCoroutineDescriptor = NULL;

	out_pPipeStreamState->mpCurrentBuffer = NULL;
	out_pPipeStreamState->mCurrentBufferSize = 0;
}

size_t pipeStreamRead(void *in_out_pPipeStreamState, void *out_pBuffer, size_t in_count)
{
	PipeStreamState *pPipeStreamState = (PipeStreamState*) in_out_pPipeStreamState;
	uint8_t *pBuffer = (uint8_t*) out_pBuffer;
	size_t bytesRead = 0;

	// This means we want to terminate...
	if (0 == in_count)
	{
		xchg(
			&pPipeStreamState->mpCurrentCoroutineDescriptor, 
			&pPipeStreamState->mpOtherCoroutineDescriptor,
			sizeof(CoroutineDescriptor*));
		switchToCoroutine(pPipeStreamState->mpOtherCoroutineDescriptor,
			pPipeStreamState->mpCurrentCoroutineDescriptor);

		return 0;
	}

	while (bytesRead < in_count)
	{
		size_t bytesToReadInCurrentIteration = MIN(in_count-bytesRead, pPipeStreamState->mCurrentBufferSize);

		if (0 == bytesToReadInCurrentIteration)
		{
			xchg(
				&pPipeStreamState->mpCurrentCoroutineDescriptor, 
				&pPipeStreamState->mpOtherCoroutineDescriptor,
				sizeof(CoroutineDescriptor*));
			switchToCoroutine(pPipeStreamState->mpOtherCoroutineDescriptor,
				pPipeStreamState->mpCurrentCoroutineDescriptor);

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

	/*
	* Note that pPipeStreamState->mCurrentBufferSize becomes != 0
	* exactly in the case of calling pipeStreamRead([...], [...], 0)
	* when pPipeStreamState->mCurrentBufferSize != 0.
	*/
	
	pPipeStreamState->mpCurrentBuffer = (const uint8_t*) in_pBuffer;
	pPipeStreamState->mCurrentBufferSize = in_count;

	xchg(
		&pPipeStreamState->mpCurrentCoroutineDescriptor, 
		&pPipeStreamState->mpOtherCoroutineDescriptor,
		sizeof(CoroutineDescriptor*));
	switchToCoroutine(pPipeStreamState->mpOtherCoroutineDescriptor,
		pPipeStreamState->mpCurrentCoroutineDescriptor);

	return in_count - pPipeStreamState->mCurrentBufferSize;
}
