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

void setjmpStreamInit(SetjmpStream *out_pSetjmpStream, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pSuperByteStream)
{
	ByteStreamInterface* pSuperByteStream = (ByteStreamInterface*) in_pSuperByteStream;
	
	// Because of PRE:SetjmpStream_h:36
	assert(in_longjmpValue != 0);

	memset(&out_pSetjmpStream->mByteStreamInterface, 0, sizeof(out_pSetjmpStream->mByteStreamInterface));

	if (pSuperByteStream->mpfRead)
		out_pSetjmpStream->mByteStreamInterface.mpfRead = setjmpStreamRead;

	if (pSuperByteStream->mpfWrite)
		out_pSetjmpStream->mByteStreamInterface.mpfWrite = setjmpStreamWrite;

	out_pSetjmpStream->mpJmpBuffer = in_pJmpBuffer;
	out_pSetjmpStream->mLongjmpValue = in_longjmpValue;
	out_pSetjmpStream->mpSuperByteStream = in_pSuperByteStream;
}

size_t setjmpStreamRead(void *in_out_pSetjmpStream, void *out_pBuffer, size_t in_count)
{
	SetjmpStream *pSetjmpStream = (SetjmpStream*) in_out_pSetjmpStream;

	if (pSetjmpStream->mpSuperByteStream->mpfRead)
	{
		if (pSetjmpStream->mpSuperByteStream->mpfRead(pSetjmpStream->mpSuperByteStream, out_pBuffer, in_count) != in_count)
			longjmp(*pSetjmpStream->mpJmpBuffer, pSetjmpStream->mLongjmpValue);

		return in_count;
	}
	else
	{
		return 0;
	}
}

size_t setjmpStreamWrite(void *in_out_pSetjmpStream, const void *out_pBuffer, size_t in_count)
{
	SetjmpStream *pSetjmpStream = (SetjmpStream*) in_out_pSetjmpStream;

	if (pSetjmpStream->mpSuperByteStream->mpfWrite)
	{
		if (pSetjmpStream->mpSuperByteStream->mpfWrite(pSetjmpStream->mpSuperByteStream, out_pBuffer, in_count) != in_count)
			longjmp(*pSetjmpStream->mpJmpBuffer, pSetjmpStream->mLongjmpValue);

		return in_count;
	}
	else
	{
		return 0;
	}
}

int setjmpStreamXchgAndSetjmp(SetjmpStream *in_out_pSetjmpStream, 
	jmp_buf *in_pJmpBuffer)
{
	xchg(*in_out_pSetjmpStream->mpJmpBuffer, in_pJmpBuffer, sizeof(jmp_buf*));
	return setjmp(*in_out_pSetjmpStream->mpJmpBuffer);
}

void setjmpStreamXchgAndLongjmp(SetjmpStream *in_out_pSetjmpStream, 
	jmp_buf *in_pJmpBuffer)
{
	xchg(in_out_pSetjmpStream->mpJmpBuffer, in_pJmpBuffer, sizeof(jmp_buf*));
	longjmp(*in_out_pSetjmpStream->mpJmpBuffer, in_out_pSetjmpStream->mLongjmpValue);
}
