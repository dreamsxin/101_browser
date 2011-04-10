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

#include "MiniStdlib/xchg.h"
#include "IO/PipeStream.h"

void xchgAndSwitchCoroutine(void *in_out_pPipeStreamState)
{
	PipeStreamState *pPipeStreamState = (PipeStreamState *) in_out_pPipeStreamState;
	
	xchg(&pPipeStreamState->mpCurrentCoroutineDescriptor, 
		&pPipeStreamState->mpOtherCoroutineDescriptor, sizeof(CoroutineDescriptor*));
	/*
	* Note that we just exchanged the two coroutine descriptors; that's why we
	* have this strange order.
	*/
	switchToCoroutine(pPipeStreamState->mpOtherCoroutineDescriptor, pPipeStreamState->mpCurrentCoroutineDescriptor);
}

void pipeStreamTerminalLoopRead(void *in_out_pPipeStreamState)
{
	// equals pipeStreamRead(in_out_pPipeStreamState, NULL, 0);

	(((CoroutineStreamFunctions*) in_out_pPipeStreamState)->mpfSwitchCoroutine)(in_out_pPipeStreamState);
}

void pipeStreamTerminalLoopWrite(void *in_out_pPipeStreamState)
{
	// equals pipeStreamWrite(in_out_pPipeStreamState, NULL, 0);

	((PipeStreamState*) in_out_pPipeStreamState)->mpCurrentBuffer = NULL;
	((PipeStreamState*) in_out_pPipeStreamState)->mCurrentBufferSize = 0;

	(((CoroutineStreamFunctions*) in_out_pPipeStreamState)->mpfSwitchCoroutine)(in_out_pPipeStreamState);
}

bool initPipeStreamState(PipeStreamState *out_pPipeStreamState,
	bool in_isOtherStreamReader,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(void *in_pStreamState, void *in_pUserData),
	void *in_pUserData)
{
	//CoroutineStateAndKickoff stateAndKickoff;
	
	out_pPipeStreamState->mFunctions.mpfSwitchCoroutine = xchgAndSwitchCoroutine;

	out_pPipeStreamState->mpCurrentBuffer = NULL;
	out_pPipeStreamState->mCurrentBufferSize = 0;

	out_pPipeStreamState->mpCurrentCoroutineDescriptor = out_pThisCoroutine;
	out_pPipeStreamState->mpOtherCoroutineDescriptor = out_pOtherCoroutine;

	return coroutineStreamStart(out_pPipeStreamState,
		out_pThisCoroutine,
		out_pOtherCoroutine,
		in_isOtherStreamReader ? pipeStreamTerminalLoopRead : pipeStreamTerminalLoopWrite,
		in_pOtherCoroutineStartup,
		in_pUserData);
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

	return coroutineStreamRead(
		in_out_pPipeStreamState, 
		out_pBuffer, in_count, 
		&pPipeStreamState->mpCurrentBuffer, &pPipeStreamState->mCurrentBufferSize);
}

size_t pipeStreamWrite(void *in_out_pPipeStreamState, const void *in_pBuffer, size_t in_count)
{
	PipeStreamState *pPipeStreamState = (PipeStreamState*) in_out_pPipeStreamState;
	
	/*
	* The last parameter is false since we don't have to reset the buffer
	* (although it wouldn't mind)
	*/
	return coroutineStreamWrite(
		in_out_pPipeStreamState, 
		in_pBuffer, in_count, 
		&pPipeStreamState->mpCurrentBuffer, &pPipeStreamState->mCurrentBufferSize, 
		false);
}
