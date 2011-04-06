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

#ifndef _MTAx_Coroutine_Coroutine_h
#define _MTAx_Coroutine_Coroutine_h

#include "MiniStdlib/declspec.h"

#if defined(_WIN32)
# include "CoroutineWin/CoroutineWin.h"
#elif defined(__unix)
# include "CoroutinePosix/CoroutinePosix.h"
#else
# error Your OS seems not to be supported
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
* Return value:
* true - sucess
* false - failure
*/
DLLEXPORT bool createCoroutine(size_t in_stackSize, 
	void (COROUTINE_KICKOFF_CALL * in_pFiberFunc)(void*), 
	void* in_pParam, 
	volatile CoroutineDescriptor *out_pCoroutineDescriptor);

/*!
* Switches to the coroutine in_pNextCoroutine
* Under Posix you will have to pass in_pCurrentCoroutine since we have to store
* the current context.
* Under Windows we may pass NULL here.
*/
DLLEXPORT bool switchToCoroutine(
	CoroutineDescriptor *in_pCurrentCoroutine, 
	CoroutineDescriptor *in_pNextCoroutine);

/*!
* Return value:
* true - sucess
* false - failure
*/
DLLEXPORT bool convertThreadToCoroutine(
	CoroutineDescriptor *out_pCoroutineDescriptor);

/*!
* Return value:
* true - sucess
* false - failure
*/
DLLEXPORT bool convertCoroutineToThread();

/*!
* Warning: Under Posix you must never call it on a coroutine 
* that was not created by createCoroutine.
* 
* You have been warned.
*/
DLLEXPORT void deleteCoroutine(CoroutineDescriptor *in_pCoroutine);

#ifdef __cplusplus
}
#endif

#endif
