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

#ifndef _MTAx_SetjmpUtil_ConditionalLongjmp_h
#define _MTAx_SetjmpUtil_ConditionalLongjmp_h

#include "SetjmpUtil/SetjmpUtil.h"
#include "MiniStdlib/cstdbool.h" // for bool
#include "MiniStdlib/cstddef.h"  // for size_t

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT void printHandler(void *in_pString);

DLLEXPORT void longjmpIf(bool condition, jmp_buf *in_pJmpBuffer, 
	int in_longjmpValue, void (*in_pfLongjmpHandlerFunction)(void *), 
	void *in_pLongjmpHandlerParam);

DLLEXPORT void longjmpWithHandler(jmp_buf *in_pJmpBuffer, 
	int in_longjmpValue, void (*in_pfLongjmpHandlerFunction)(void *), 
	void *in_pLongjmpHandlerParam);

DLLEXPORT void setjmpStateLongjmpIf(SetjmpState *in_out_pSetjmpState, 
	bool condition, void *in_pLongjmpHandlerParam);

DLLEXPORT void* longjmpMalloc(jmp_buf *in_pJmpBuffer, 
	int in_longjmpValue, size_t in_size);

DLLEXPORT void* setjmpStateLongjmpMalloc(SetjmpState *in_out_pSetjmpState, 
	size_t in_size);

#ifdef __cplusplus
}
#endif

#endif
