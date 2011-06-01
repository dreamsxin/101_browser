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
#include <string.h> // for memset
#include "MiniStdlib/xchg.h"

void setjmpStreamInit(SetjmpStreamState *out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pSuperByteStreamState, ByteStreamInterface in_superByteStreamInterface)
{
	// Because of PRE:SetjmpStream_h:36
	assert(in_longjmpValue != 0);

	out_pSetjmpStreamState->mpJmpBuffer = in_pJmpBuffer;
	out_pSetjmpStreamState->mLongjmpValue = in_longjmpValue;
	out_pSetjmpStreamState->mpSuperByteStreamState = in_pSuperByteStreamState;
	
	out_pSetjmpStreamState->mSuperByteStreamInterface = in_superByteStreamInterface;
}

size_t setjmpStreamRead(void *in_out_pSetjmpStreamState, void *out_pBuffer, size_t in_count)
{
	SetjmpStreamState *pSetjmpStreamState = (SetjmpStreamState*) in_out_pSetjmpStreamState;

	if (pSetjmpStreamState->mSuperByteStreamInterface.mpfRead != NULL)
	{
		if (pSetjmpStreamState->mSuperByteStreamInterface.mpfRead(pSetjmpStreamState->mpSuperByteStreamState, 
			out_pBuffer, in_count) != in_count)
			longjmp(*pSetjmpStreamState->mpJmpBuffer, pSetjmpStreamState->mLongjmpValue);

		return in_count;
	}
	else
	{
		return 0;
	}
}

size_t setjmpStreamWrite(void *in_out_pSetjmpStreamState, const void *out_pBuffer, size_t in_count)
{
	SetjmpStreamState *pSetjmpStreamState = (SetjmpStreamState*) in_out_pSetjmpStreamState;

	if (pSetjmpStreamState->mSuperByteStreamInterface.mpfWrite != NULL)
	{
		if (pSetjmpStreamState->mSuperByteStreamInterface.mpfWrite(pSetjmpStreamState->mpSuperByteStreamState, 
			out_pBuffer, in_count) != in_count)
			longjmp(*pSetjmpStreamState->mpJmpBuffer, pSetjmpStreamState->mLongjmpValue);

		return in_count;
	}
	else
	{
		return 0;
	}
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

DLLEXPORT ByteStreamInterface getSetjmpStreamByteStreamInterface(const SetjmpStreamState * in_pcSetjmpStreamState)
{
	ByteStreamInterface out_interface;
	memset(&out_interface, 0, sizeof(out_interface));

	if (in_pcSetjmpStreamState->mSuperByteStreamInterface.mpfRead)
		out_interface.mpfRead = setjmpStreamRead;

	if (in_pcSetjmpStreamState->mSuperByteStreamInterface.mpfWrite)
		out_interface.mpfWrite = setjmpStreamWrite;

	// TODO Write support for seek

	return out_interface;
}
