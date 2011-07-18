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
	void (*mpfLongjmpHandlerFunction)(void *);
} SetjmpState;

DLLEXPORT void setjmpStateInit(SetjmpState *out_pSetjmpState,
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void (*in_pfLongjmpHandlerFunction)(void *));

/*!
* These following two functions are intended for "catching the exception" 
* for freeing allocated ressources before rethrowing it.
*/

/*!
* Does an exchange of jmp_buf-s (if NULL == in_pJmpBuffer there will be no 
* exchange). Then the function does a setjmp.
* 
* Parameters:
* in_out_pSetjmpState: pointer to the "old" SetjmpState 
*                      (NULL != in_out_pSetjmpState)
* in_pJmpBuffer:       pointer to the "new" jmp_buf that will be used from now 
*                      on (may also be NULL)
*/
DLLEXPORT int setjmpStateXchgAndSetjmp(SetjmpState *in_out_pSetjmpState,
	jmp_buf *in_pJmpBuffer);

/*!
* Does an exchange of jmp_buf-s (if NULL == in_pJmpBuffer there will be no 
* exchange). If there is a handler function defined it will
* be called after with parameter in_pLongjmpHandlerParam.
* Then the function does a longjmp.
* 
* Parameters:
* in_out_pSetjmpState:     pointer to the "new" SetjmpState that will be used 
*                          from now on (NULL != in_out_pSetjmpState)
* in_pJmpBuffer:           pointer to the "old" jmp_buf (may also be NULL)
* in_pLongjmpHandlerParam: the parameter to pass to the handler function
*/
DLLEXPORT void setjmpStateXchgAndLongjmp(SetjmpState *in_out_pSetjmpState,
	jmp_buf *in_pJmpBuffer, void *in_pLongjmpHandlerParam);

#ifdef __cplusplus
}
#endif

#endif
