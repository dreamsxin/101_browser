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

#ifndef _IO_SetjmpStream_h
#define _IO_SetjmpStream_h

#include "MiniStdlib/declspec.h"
#include "SetjmpUtil/SetjmpUtil.h"
#include "IO/ByteStream.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	SetjmpState setjmpState;
	void *mpSuperByteStreamState;
	ByteStreamInterface mSuperByteStreamInterface;
} SetjmpStreamState;

/*!
* setjmpStreamInit
*
* Initialize in_pSetjmpStreamState. Does *not* call setjmp.
*
* Precondition: in_longjmpValue != 0 (because of limitations
* of setjmp/longjmp this is surely not what you want).
* PRE:SetjmpStream_h:36
*/
DLLEXPORT void setjmpStreamInit(SetjmpStreamState *out_pSetjmpStreamState, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pSuperByteStreamState, ByteStreamInterface in_superByteStreamInterface);

DLLEXPORT size_t setjmpStreamRead(void *in_out_pSetjmpStreamState, 
	void *out_pBuffer, size_t in_count);

DLLEXPORT size_t setjmpStreamWrite(void *in_out_pSetjmpStreamState, 
	const void *out_pBuffer, size_t in_count);

DLLEXPORT ByteStreamInterface getSetjmpStreamByteStreamInterface(const SetjmpStreamState * in_pcSetjmpStreamState);

#ifdef __cplusplus
}
#endif

#endif
