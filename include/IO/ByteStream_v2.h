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

#ifndef _MTAx_IO_ByteStream_v2_h
#define _MTAx_IO_ByteStream_v2_h

#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void (*mpfRead)(void *in_out_pByteStreamInterface, 
		void *out_pBuffer, size_t in_count, bool in_isTerminal, 
		size_t *out_pCount, bool *out_pIsTerminal);
	void (*mpfWrite)(void *in_out_pByteStreamInterface, 
		const void *in_pBuffer, size_t in_count, bool in_isTerminal, 
		size_t *out_pCount, bool *out_pIsTerminal);
} ByteStreamInterface_v2;

#ifdef __cplusplus
}
#endif

#endif
