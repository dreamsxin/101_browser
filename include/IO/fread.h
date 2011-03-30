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

#ifndef _fread_h
#define _fread_h

#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT size_t fread_withState(void *in_dstBuffer, size_t in_elementSize, size_t in_count, FILE* in_file,
	void *in_pState, void (*in_pHandler)(void* in_pState, const void* in_pBuffer, size_t in_count));

#ifdef __cplusplus
}
#endif

#endif