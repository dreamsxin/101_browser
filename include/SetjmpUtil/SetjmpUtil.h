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
* If there is a handler function defined it will
* be called after with parameter in_pLongjmpHandlerParam.
* Then the function does a longjmp.
* 
* Parameters:
* in_out_pSetjmpState:     pointer to the SetjmpState that will be used 
*                          from now on (NULL != in_out_pSetjmpState)
* in_pLongjmpHandlerParam: the parameter to pass to the handler function
*/
DLLEXPORT void setjmpStateLongjmp(SetjmpState *in_out_pSetjmpState,
	void *in_pLongjmpHandlerParam);


/*!
* Exchanges the jmp_buf-s (note that jmp_buf is an array type).
* 
* Preconditions: 
* NULL != in_pJmpBuf0
* NULL != in_pJmpBuf1
*/
DLLEXPORT void xchgJmpBuf(jmp_buf in_jmpBuf0, jmp_buf in_jmpBuf1);

/*!
* These following two functions are intended for "catching the exception" 
* for freeing allocated ressources before rethrowing it.
*/

/*!
* Does an exchange of jmp_buf-s. Then the macro does a setjmp.
* 
* Parameters:
* in_pPrevJmpBuf: the jmp_buf that was previously used
* in_pNextJmpBuf: the jmp_buf that shall be used next
*
* Q: Why is this a macro and not a function?
* A: If it was a function, we would store a value that is invalid
*    after leaving the function.
*/
#define XCHG_AND_SETJMP(in_prevJmpBuf, in_nextJmpBuf) ( \
	xchgJmpBuf(in_prevJmpBuf, in_nextJmpBuf), \
	setjmp(in_prevJmpBuf))


/*!
* Does an exchange of the jmp_buf-s.
* Then the function does a longjmp.
* 
* Parameters:
* in_pCurrJmpBuf: pointer to the jmp_buf that we came from in xchgAndSetjmp
*                 (!= NULL)
* in_pPrevJmpBuf: the jmp_buf that was previously used (!= NULL)
* in_value:       the parameter to pass to longjmp (0 != in_value)
*/

DLLEXPORT void xchgAndLongjmp(jmp_buf in_pCurrJmpBuf,
	jmp_buf in_pPrevJmpBuf, int in_value);

#ifdef __cplusplus
}
#endif

#endif
