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

typedef struct
{
	void (*mpfSwitchCoroutine)(void *);
} CoroutineStreamFunctions;

size_t coroutineStreamRead(void *in_out_pStreamState, 
	void *out_pBuffer, size_t in_count, 
	const uint8_t * volatile *in_ppCurrentBuffer, volatile size_t * in_pCurrentBufferSize);

size_t coroutineStreamWrite(void *in_out_pStreamState, 
	const void *in_pBuffer, size_t in_count, 
	const uint8_t * volatile *in_ppCurrentBuffer, volatile size_t * in_pCurrentBufferSize);

#endif
