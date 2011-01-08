/*
 * Copyright 2008-2011 Wolfgang Keller
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

#ifndef _MTAx_RFC1950_adler32_h
#define _MTAx_RFC1950_adler32_h

/* 
 * gcc requires this header file for a definition of size_t
 */
#ifdef __GNUC__
# ifdef __cplusplus
#include <cstddef>
# else
#include <stddef.h>
# endif
#endif

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t RFC1950_adler32_init();

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t RFC1950_adler32_compute(void *in_buffer, size_t in_bufferSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t RFC1950_adler32_foldl(uint32_t in_adler, uint8_t *in_buffer, size_t in_bufferSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t RFC1950_adler32_update(uint32_t in_adler, uint8_t in_currentByte);

#ifdef __cplusplus
}
#endif

#endif
