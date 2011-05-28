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

#include "MiniStdlib/csetjmp.h"
#include "MiniStdlib/declspec.h"
#include "IO/ByteStreamInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	ByteStreamInterface mByteStreamInterface;
	ByteStreamInterface *mpSuperByteStream;
	jmp_buf *mpJmpBuffer;
	int mLongjmpValue;
} SetjmpStream;


/*!
* setjmpReadStreamInit and setjmpWriteStreamInit
*
* Initialize in_pSetjmpStream. Does *not* call setjmp.
*
* Precondition: in_longjmpValue != 0 (because of limitations
* of setjmp/longjmp this is surely not what you want).
* PRE:SetjmpStream_h:36
*/
DLLEXPORT void setjmpStreamInit(SetjmpStream *out_pSetjmpStream, 
	jmp_buf *in_pJmpBuffer, int in_longjmpValue, 
	void *in_pSuperByteStream);

DLLEXPORT size_t setjmpStreamRead(void *in_out_pSetjmpStream, 
	void *out_pBuffer, size_t in_count);

DLLEXPORT size_t setjmpStreamWrite(void *in_out_pSetjmpStream, 
	const void *out_pBuffer, size_t in_count);

/*!
* These two functions are intended for "catching the exception" for freeing 
* allocated ressources before rethrowing it.
*/
DLLEXPORT int setjmpStreamXchgAndSetjmp(SetjmpStream *in_out_pSetjmpStream, 
	jmp_buf *in_pJmpBuffer);

DLLEXPORT void setjmpStreamXchgAndLongjmp(SetjmpStream *in_out_pSetjmpStream, 
	jmp_buf *in_pJmpBuffer);

#ifdef __cplusplus
}
#endif

#endif
