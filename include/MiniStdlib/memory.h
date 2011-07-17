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

#ifndef _MTAx_MiniStdlib_memory_h
#define _MTAx_MiniStdlib_memory_h

#include "MiniStdlib/cstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
* Sets 16 bits a time instead of 8, so the memory block has to have a size of
* 2*in_n bytes
*
* Return value: in_out_pBuffer
*/
void* memset2(void *in_out_pBuffer, int in_value, size_t in_num);


/*!
* Exchanges the memory blocks pointed to in_p0 and in_p1 having
* size of in_num bytes.
*
* Assumption: these memory block do not alias.
*/
void memxchg(void *in_p0, void *in_p1, size_t in_num);


/*!
* "Reverses" the memory block pointed to by in_out_pBuffer
* with size in_num.
*
* Return value: in_out_pBuffer
*/
void* endianness_convert(void *in_out_pBuffer, size_t in_num);

/*!
* A shortcut for the (probably) most often used version of endianness_convert.
*/
#define ENDIANNESS_CONVERT_SIMPLE(x) endianness_convert(&x, sizeof(x));

#ifdef __cplusplus
}
#endif

#endif
