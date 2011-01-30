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

#ifndef _MTAx_Algorithm_Bitmask_h
#define _MTAx_Algorithm_Bitmask_h

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/cstddef.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
bool checkBitmask(const void *in_pBuffer, size_t in_bufferSize, const void *in_pMaskIdentifierBits, const void *in_pMaskValueBits);

#ifdef __cplusplus
}
#endif

#endif
