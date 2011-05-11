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

#include "IO/CoroutineStream.h"
#include "MiniStdlib/minmax.h"
#include <string.h>

void COROUTINE_KICKOFF_CALL coroutineStreamKickoff(void *in_pCoroutineStateAndKickoff)
{
	CoroutineStateAndKickoff stateAndKickoff = *(CoroutineStateAndKickoff*) in_pCoroutineStateAndKickoff;

	/*
	* No exchanging of descriptors since it hasn't been done originally when switching to
	* this coroutine.
	*/
	(((CoroutineStreamFunctions*) stateAndKickoff.pState)->mpfSwitchCoroutine)(stateAndKickoff.pState);

	(*stateAndKickoff.kickoffData.mpStartup)(stateAndKickoff.pState, stateAndKickoff.kickoffData.mpUserData);

	if (stateAndKickoff.kickoffData.mpTerminateFun)
		(*stateAndKickoff.kickoffData.mpTerminateFun)(stateAndKickoff.pState);

	while (1)
	{
		(*stateAndKickoff.kickoffData.mpTerminalLoopFun)(stateAndKickoff.pState);
	}
}

bool coroutineStreamStart(void *in_pStreamState, 
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(void *in_pStreamState, void *in_pUserData),
	void (*in_pfTerminate)(void *in_out_pStreamState),
	void (*in_pfTerminalLoopFunction)(void *in_out_pStreamState),
	void *in_pUserData)
{
	CoroutineStateAndKickoff stateAndKickoff;
	
	stateAndKickoff.pState = in_pStreamState;
	stateAndKickoff.kickoffData.mpStartup = in_pOtherCoroutineStartup;
	stateAndKickoff.kickoffData.mpTerminateFun = in_pfTerminate;
	stateAndKickoff.kickoffData.mpTerminalLoopFun = in_pfTerminalLoopFunction;
	stateAndKickoff.kickoffData.mpUserData = in_pUserData;

	if (!convertThreadToCoroutine(out_pThisCoroutine))
		return false;

	if (!createCoroutine(0, coroutineStreamKickoff, &stateAndKickoff, out_pOtherCoroutine))
		return false;

	(((CoroutineStreamFunctions*) in_pStreamState)->mpfSwitchCoroutine)(in_pStreamState);

	return true;
}

size_t coroutineStreamRead(void *in_out_pStreamState, 
	void *out_pBuffer, size_t in_count, 
	const uint8_t * volatile *in_ppCurrentBuffer, volatile size_t * in_pCurrentBufferSize)
{
	uint8_t *pBuffer = (uint8_t*) out_pBuffer;
	size_t bytesRead = 0;
	size_t bytesToReadInCurrentIterationCount = MIN(in_count-bytesRead, *in_pCurrentBufferSize);

	if (0 == bytesToReadInCurrentIterationCount)
		goto switch_to_writer;

	while (1)
	{
		// copy bytes
		memcpy(pBuffer, *in_ppCurrentBuffer, bytesToReadInCurrentIterationCount);

		bytesRead += bytesToReadInCurrentIterationCount;
		pBuffer += bytesToReadInCurrentIterationCount;
		*in_ppCurrentBuffer += bytesToReadInCurrentIterationCount;
		*in_pCurrentBufferSize -= bytesToReadInCurrentIterationCount;

		if (*in_pCurrentBufferSize > 0 || bytesRead == in_count)
			break;


		// switch to writer
switch_to_writer:

		(((CoroutineStreamFunctions *) in_out_pStreamState)->mpfSwitchCoroutine)(in_out_pStreamState);

		bytesToReadInCurrentIterationCount = MIN(in_count-bytesRead, *in_pCurrentBufferSize);

		if (bytesToReadInCurrentIterationCount == 0)
			break;
	}

	return bytesRead;
}

size_t coroutineStreamWrite(
	void *in_out_pStreamState, 
	const void *in_pBuffer, size_t in_count, 
	const uint8_t * volatile *in_ppCurrentBuffer, volatile size_t * in_pCurrentBufferSize, 
	bool in_resetBuffer)
{
	size_t out_writtenCount;
	
	*in_ppCurrentBuffer = (const uint8_t*) in_pBuffer;
	*in_pCurrentBufferSize = in_count;

	((CoroutineStreamFunctions*) in_out_pStreamState)->mpfSwitchCoroutine(in_out_pStreamState);
	out_writtenCount = in_count - *in_pCurrentBufferSize;

	if (in_resetBuffer)
	{
		*in_ppCurrentBuffer = NULL;
		*in_pCurrentBufferSize = 0;
	}

	return out_writtenCount;
}
