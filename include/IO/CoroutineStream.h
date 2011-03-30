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

#ifndef _MTAx_IO_CoroutineStream_h
#define _MTAx_IO_CoroutineStream_h

#include "MiniStdlib/cstdint.h"
#include "Coroutine/Coroutine.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void (*mpfSwitchCoroutine)(void *);
} CoroutineStreamFunctions;

typedef struct
{
	void (*mpStartup)(void *in_pStreamState, void *in_pUserData);
	void (*mpTerminateLoopFun)(void*);
	// Data to pass to the other coroutine
	void *mpUserData;
} CoroutineStreamKickoffData;

typedef struct
{
	void *pState;
	CoroutineStreamKickoffData kickoffData;
} CoroutineStateAndKickoff;

void COROUTINE_KICKOFF_CALL coroutineStreamKickoff(void *in_pCoroutineStateAndKickoff);

bool coroutineStreamStart(void *in_pStreamState, 
	CoroutineDescriptor *out_pThisCoroutine,
	CoroutineDescriptor *out_pOtherCoroutine,
	void (*in_pfTerminalFunction)(void *in_out_pStreamState),
	void (*in_pOtherCoroutineStartup)(void *in_pStreamState, void *in_pUserData),
	void *in_pUserData);

size_t coroutineStreamRead(void *in_out_pStreamState, 
	void *out_pBuffer, size_t in_count, 
	const uint8_t * volatile *in_ppCurrentBuffer, volatile size_t * in_pCurrentBufferSize);

size_t coroutineStreamWrite(void *in_out_pStreamState, 
	const void *in_pBuffer, size_t in_count, 
	const uint8_t * volatile *in_ppCurrentBuffer, volatile size_t * in_pCurrentBufferSize);

#ifdef __cplusplus
}
#endif

#endif
