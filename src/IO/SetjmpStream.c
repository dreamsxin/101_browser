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

void setjmpReadStreamInit(SetjmpReadStreamState *out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pByteStreamState, ByteStreamReadInterface in_readInterface)
{
	setjmpStreamUtilDataInit(&out_pSetjmpStreamState->utilData, 
		in_pJmpBuffer, in_longjmpValue, in_pByteStreamState);
	out_pSetjmpStreamState->mReadInterface = in_readInterface;
}

void setjmpWriteStreamInit(SetjmpWriteStreamState *out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pByteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	setjmpStreamUtilDataInit(&out_pSetjmpStreamState->utilData, 
		in_pJmpBuffer, in_longjmpValue, in_pByteStreamState);
	out_pSetjmpStreamState->mWriteInterface = in_writeInterface;
}

void setjmpStreamUtilDataInit(SetjmpStreamUtilData *out_pSetjmpStreamUtilData, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, void *in_pByteStreamState)
{
	// Because of PRE:SetjmpStream_h:36
	assert(in_longjmpValue != 0);

	out_pSetjmpStreamUtilData->mpJmpBuffer = in_pJmpBuffer;
	out_pSetjmpStreamUtilData->mLongjmpValue = in_longjmpValue;
	out_pSetjmpStreamUtilData->pByteStreamState = in_pByteStreamState;
}

size_t setjmpStreamRead(void *in_out_pSetjmpStreamState, void *out_pBuffer, size_t in_count)
{
	SetjmpReadStreamState *pSetjmpReadStreamState = (SetjmpReadStreamState*) in_out_pSetjmpStreamState;

	if (pSetjmpReadStreamState->mReadInterface.pRead(pSetjmpReadStreamState->utilData.pByteStreamState, 
		out_pBuffer, in_count) != in_count)
		longjmp(*pSetjmpReadStreamState->utilData.mpJmpBuffer, pSetjmpReadStreamState->utilData.mLongjmpValue);

	return in_count;
}

size_t setjmpStreamWrite(void *in_out_pSetjmpStreamState, const void *out_pBuffer, size_t in_count)
{
	SetjmpWriteStreamState *pSetjmpWriteStreamState = (SetjmpWriteStreamState*) in_out_pSetjmpStreamState;

	if (pSetjmpWriteStreamState->mWriteInterface.pWrite(pSetjmpWriteStreamState->utilData.pByteStreamState, 
		out_pBuffer, in_count) != in_count)
		longjmp(*pSetjmpWriteStreamState->utilData.mpJmpBuffer, pSetjmpWriteStreamState->utilData.mLongjmpValue);

	return in_count;
}

int setjmpStreamXchgAndSetjmp(SetjmpStreamUtilData *in_out_pSetjmpStreamUtilData, 
	jmp_buf *in_pJmpBuffer)
{
	xchg(*in_out_pSetjmpStreamUtilData->mpJmpBuffer, in_pJmpBuffer, sizeof(jmp_buf*));
	return setjmp(*in_out_pSetjmpStreamUtilData->mpJmpBuffer);
}

void setjmpStreamXchgAndLongjmp(SetjmpStreamUtilData *in_out_pSetjmpStreamUtilData, 
	jmp_buf *in_pJmpBuffer)
{
	xchg(in_out_pSetjmpStreamUtilData->mpJmpBuffer, in_pJmpBuffer, sizeof(jmp_buf*));
	longjmp(*in_out_pSetjmpStreamUtilData->mpJmpBuffer, in_out_pSetjmpStreamUtilData->mLongjmpValue);
}

ByteStreamReadInterface getSetjmpReadStreamReadInterface()
{
	ByteStreamReadInterface out_interface = { setjmpStreamRead };
	return out_interface;
}

ByteStreamWriteInterface getSetjmpWriteStreamReadInterface()
{
	ByteStreamWriteInterface out_interface = { setjmpStreamWrite };
	return out_interface;
}
