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

#ifndef _MTAx_SetjmpUtil_SetjmpUtil_h
#define _MTAx_SetjmpUtil_SetjmpUtil_h

#include "MiniStdlib/csetjmp.h"
#include "MiniStdlib/declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	jmp_buf *mpJmpBuffer;
	int mLongjmpValue;
} SetjmpState;

DLLEXPORT void setjmpStateInit(SetjmpState *in_pSetjmpState,
	jmp_buf *in_pJmpBuffer, int in_longjmpValue);

/*!
* These two functions are intended for "catching the exception" for freeing 
* allocated ressources before rethrowing it.
*/
DLLEXPORT int setjmpStateXchgAndSetjmp(SetjmpState *in_out_pSetjmpState,
	jmp_buf *in_pJmpBuffer);

DLLEXPORT void setjmpStateXchgAndLongjmp(SetjmpState *in_out_pSetjmpState,
	jmp_buf *in_pJmpBuffer);

#ifdef __cplusplus
}
#endif

#endif
