/*
* Copyright 2008-2012 Wolfgang Keller
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

#ifndef _MTAx_IO_ByteStream_v4_h
#define _MTAx_IO_ByteStream_v4_h

#include "MiniStdlib/cstddef.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	ByteStreamStatus_OK,
	ByteStreamStatus_Terminated,
	ByteStreamStatus_InterventionRequired,
	ByteStreamStatus_Error
} ByteStreamStatus_v4;

typedef struct
{
	ByteStreamStatus_v4 (*mpfGetStatus)(const void *in_pByteStreamState);
	void (*mpfSetStatus)(void *in_out_pByteStreamState, ByteStreamStatus_v4 in_status);
} CommonByteStreamInterface_v4;

typedef struct
{
	CommonByteStreamInterface_v4 commonByteStreamInterface;
	size_t (*mpfRead)(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count);
} ByteStreamReadInterface_v4;

typedef struct
{
	CommonByteStreamInterface_v4 commonByteStreamInterface;
	size_t (*mpfWrite)(void *in_out_pByteStreamState, 
		const void *in_pBuffer, size_t in_count);
} ByteStreamWriteInterface_v4;

#ifdef __cplusplus
}
#endif

#endif
