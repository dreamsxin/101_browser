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

#include "SetjmpUtil/SetjmpUtil.h"
#include "MiniStdlib/memory.h"  // for memxchg
#include "MiniStdlib/cassert.h" // guess for what ;-)

void setjmpStateInit(SetjmpState *out_pSetjmpState,
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void (*in_pfLongjmpHandlerFunction)(void *))
{
	out_pSetjmpState->mpJmpBuffer = in_pJmpBuffer;
	out_pSetjmpState->mLongjmpValue = in_longjmpValue;
	out_pSetjmpState->mpfLongjmpHandlerFunction = in_pfLongjmpHandlerFunction;
}

void setjmpStateLongjmp(SetjmpState *in_out_pSetjmpState,
	void *in_pLongjmpHandlerParam)
{
	assert(NULL != in_out_pSetjmpState);

	if (NULL == in_out_pSetjmpState)
		return;
	
	if (in_out_pSetjmpState->mpfLongjmpHandlerFunction)
		(*in_out_pSetjmpState->mpfLongjmpHandlerFunction)(in_pLongjmpHandlerParam);

	longjmp(*in_out_pSetjmpState->mpJmpBuffer, in_out_pSetjmpState->mLongjmpValue);
}

void xchgJmpBuf(jmp_buf in_jmpBuf0, jmp_buf in_jmpBuf1)
{
	assert(NULL != in_jmpBuf0);
	assert(NULL != in_jmpBuf1);

	memxchg(in_jmpBuf0, in_jmpBuf1, sizeof(jmp_buf));
}

void xchgAndLongjmp(jmp_buf in_currJmpBuf, jmp_buf in_prevJmpBuf, 
	int in_value)
{
	xchgJmpBuf(in_currJmpBuf, in_prevJmpBuf);

	/*
	* in_prevJmpBuf contains the "old" value again, so we may longjmp
	* on it.
	*/
	longjmp(in_prevJmpBuf , in_value);
}
