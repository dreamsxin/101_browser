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

#include "IO/SetjmpStream.h"
#include <assert.h>
#include "MiniStdlib/xchg.h"

int setjmpStreamInitAndSetjmp(
	SetjmpStreamState *out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pByteStreamState, ByteStreamReadInterface in_readInterface)
{
	// Because of PRE:SetjmpStream_h:36
	assert(in_longjmpValue != 0);

	out_pSetjmpStreamState->mpJmpBuffer = in_pJmpBuffer;
	out_pSetjmpStreamState->mLongjmpValue = in_longjmpValue;
	out_pSetjmpStreamState->pByteStreamState = in_pByteStreamState;
	out_pSetjmpStreamState->mReadInterface = in_readInterface;
	
	return setjmp(*out_pSetjmpStreamState->mpJmpBuffer);
}

size_t setjmpStreamRead(void *in_out_pSetjmpStreamState, void *out_pBuffer, size_t in_count)
{
	SetjmpStreamState *pSetjmpStreamState = (SetjmpStreamState*) in_out_pSetjmpStreamState;

	if (pSetjmpStreamState->mReadInterface.pRead(pSetjmpStreamState->pByteStreamState, out_pBuffer, in_count) != in_count)
		longjmp(*pSetjmpStreamState->mpJmpBuffer, pSetjmpStreamState->mLongjmpValue);

	return in_count;
}

int setjmpStreamXchgAndSetjmp(SetjmpStreamState *in_out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer)
{
	xchg(*in_out_pSetjmpStreamState->mpJmpBuffer, in_pJmpBuffer, sizeof(jmp_buf*));
	return setjmp(*in_out_pSetjmpStreamState->mpJmpBuffer);
}

void setjmpStreamXchgAndLongjmp(SetjmpStreamState *in_out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer)
{
	xchg(in_out_pSetjmpStreamState->mpJmpBuffer, in_pJmpBuffer, sizeof(jmp_buf*));
	longjmp(*in_out_pSetjmpStreamState->mpJmpBuffer, in_out_pSetjmpStreamState->mLongjmpValue);
}
