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
#include "MiniStdlib/xchg.h"
#include <assert.h>

void xchgAndSwitchCoroutine(void *in_out_pPipeStream)
{
	PipeStream *pPipeStream = (PipeStream*) in_out_pPipeStream;
	
	xchg(&pPipeStream->mpCurrentCoroutineDescriptor, 
		&pPipeStream->mpOtherCoroutineDescriptor, sizeof(CoroutineDescriptor*));
	/*
	* Note that we just exchanged the two coroutine descriptors; that's why we
	* have this strange order.
	*/
	switchToCoroutine(pPipeStream->mpOtherCoroutineDescriptor, pPipeStream->mpCurrentCoroutineDescriptor);
}

void pipeStreamTerminateRead(void *in_out_pPipeStreamState)
{
	// nothing
}

void pipeStreamTerminalLoopRead(void *in_out_pPipeStreamState)
{
	// The code of this function has to equal to pipeStreamWrite(in_out_pPipeStreamState, NULL, 0);

	(((CoroutineStreamInterface*) in_out_pPipeStreamState)->mpfSwitchCoroutine)(in_out_pPipeStreamState);
}

// POST:PipeStream_c_45: NULL == ((PipeStreamState*) in_out_pPipeStreamState)->mpCurrentBuffer
// POST:PipeStream_c_46: 0 == ((PipeStreamState*) in_out_pPipeStreamState)->mCurrentBufferSize
void pipeStreamTerminateWrite(void *in_out_pPipeStream)
{
	((PipeStream*) in_out_pPipeStream)->mpCurrentBuffer = NULL;
	((PipeStream*) in_out_pPipeStream)->mCurrentBufferSize = 0;
}

void pipeStreamTerminalLoopWrite(void *in_out_pPipeStream)
{
	// The code of this function has to equal to pipeStreamWrite(in_out_pPipeStreamState, NULL, 0);

	// Follows from POST:PipeStream_c_45
	assert(NULL == ((PipeStream*) in_out_pPipeStream)->mpCurrentBuffer);
	// Follows from POST:PipeStream_c_46
	assert(0 == ((PipeStream*) in_out_pPipeStream)->mCurrentBufferSize);

	(((CoroutineStreamInterface*) in_out_pPipeStream)->mpfSwitchCoroutine)(in_out_pPipeStream);
}

bool pipeStreamInit(PipeStream *out_pPipeStream,
	bool in_isOtherStreamReader,
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pOtherCoroutineStartup)(void *in_out_pStreamState, void *in_pUserData),
	void *in_pUserData)
{
	bool out_result;
	
	memset(&out_pPipeStream->mCoroutineStreamInterface.mByteStreamInterface, 0, 
		sizeof(out_pPipeStream->mCoroutineStreamInterface.mByteStreamInterface));
	out_pPipeStream->mCoroutineStreamInterface.mByteStreamInterface.mpfRead = pipeStreamRead;
	out_pPipeStream->mCoroutineStreamInterface.mByteStreamInterface.mpfWrite = pipeStreamWrite;
	out_pPipeStream->mCoroutineStreamInterface.mpfSwitchCoroutine = xchgAndSwitchCoroutine;

	out_pPipeStream->mpCurrentBuffer = NULL;
	out_pPipeStream->mCurrentBufferSize = 0;

	out_pPipeStream->mpCurrentCoroutineDescriptor = out_pThisCoroutine;
	out_pPipeStream->mpOtherCoroutineDescriptor = out_pOtherCoroutine;

	out_result = coroutineStreamStart(out_pPipeStream,
		out_pThisCoroutine,
		out_pOtherCoroutine,
		in_pOtherCoroutineStartup,
		in_isOtherStreamReader ? pipeStreamTerminateRead : pipeStreamTerminateWrite,
		in_isOtherStreamReader ? pipeStreamTerminalLoopRead : pipeStreamTerminalLoopWrite,
		in_pUserData);

	if (in_isOtherStreamReader)
	{
		(out_pPipeStream->mCoroutineStreamInterface.mpfSwitchCoroutine)(out_pPipeStream);
	}

	return out_result;
}

void pipeStreamDelete(PipeStream *out_pPipeStream)
{
	deleteCoroutine(out_pPipeStream->mpOtherCoroutineDescriptor);
	out_pPipeStream->mpOtherCoroutineDescriptor = NULL;
	out_pPipeStream->mpCurrentCoroutineDescriptor = NULL;

	out_pPipeStream->mpCurrentBuffer = NULL;
	out_pPipeStream->mCurrentBufferSize = 0;
}

size_t pipeStreamRead(void *in_out_pPipeStream, void *out_pBuffer, size_t in_count)
{
	PipeStream *pPipeStreamState = (PipeStream*) in_out_pPipeStream;

	return coroutineStreamRead(
		in_out_pPipeStream, 
		out_pBuffer, in_count, 
		&pPipeStreamState->mpCurrentBuffer, &pPipeStreamState->mCurrentBufferSize);
}

size_t pipeStreamWrite(void *in_out_pPipeStream, const void *in_pBuffer, size_t in_count)
{
	PipeStream *pPipeStream = (PipeStream*) in_out_pPipeStream;
	
	/*
	* The last parameter is false since we don't have to reset the buffer
	* (although it wouldn't mind)
	*/
	return coroutineStreamWrite(
		in_out_pPipeStream, 
		in_pBuffer, in_count, 
		&pPipeStream->mpCurrentBuffer, &pPipeStream->mCurrentBufferSize, 
		false);
}
