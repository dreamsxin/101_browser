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

#ifndef _MTAx_IO_ByteStream_v3_h
#define _MTAx_IO_ByteStream_v3_h

#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	bool (*mpfGetTerminateAfterLastOperation)(const void *in_pByteStreamState);
	bool (*mpfIsTerminated)(const void *in_pByteStreamState);
	void (*mpfTerminate)(void *out_pByteStreamState);
} CommonByteStreamInterface_v3;

typedef struct
{
	CommonByteStreamInterface_v3 commonByteStreamInterface;
	size_t (*mpfRead)(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count);
} ByteStreamReadInterface_v3;

typedef struct
{
	CommonByteStreamInterface_v3 commonByteStreamInterface;
	size_t (*mpfWrite)(void *in_out_pByteStreamState, 
		const void *in_pBuffer, size_t in_count);
} ByteStreamWriteInterface_v3;

#ifdef __cplusplus
}
#endif

#endif
