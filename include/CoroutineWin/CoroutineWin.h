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

#ifndef _MTAx_Coroutine_CoroutineWin_h
#define _MTAx_Coroutine_CoroutineWin_h

#include <windows.h>
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef LPVOID CoroutineDescriptor;
#define COROUTINE_CALL __stdcall

__declspec(dllexport) bool createCoroutine(
	size_t in_stackSize,  
	void (COROUTINE_CALL * in_pFiberFunc)(void*), 
	void* in_pParam, 
	volatile CoroutineDescriptor *out_pCoroutineDescriptor);

__declspec(dllexport) void switchToCoroutine(
	CoroutineDescriptor * in_pCurrentCoroutine, 
	CoroutineDescriptor *in_pNextCoroutine);

/*!
 * Return value:
 * true - sucess
 * false - failure
 */
__declspec(dllexport) bool convertThreadToCoroutine(
	CoroutineDescriptor *out_pCoroutineDescriptor);

__declspec(dllexport) bool convertCoroutineToThread();

// getCurrentCoroutine is only used internally
CoroutineDescriptor getCurrentCoroutine();

__declspec(dllexport) void deleteCoroutine(CoroutineDescriptor *in_pCoroutine);

#ifdef __cplusplus
}
#endif

#endif
