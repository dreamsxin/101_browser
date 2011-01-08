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

#ifndef _CRC_h
#define _CRC_h

#include "MiniStdlib/cstdint.h"
 // for size_t
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_init();

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_update(uint32_t in_currentCRC, uint8_t in_currentByte);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_terminate(uint32_t in_currentCRC);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_foldl(uint32_t in_currentCRC, uint8_t *in_buffer, size_t in_bufferSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_compute(uint8_t *in_buffer, size_t in_bufferSize);

/*!
 * The function CRC_stateUpdate is for using with fread_withState (IO/fread.h).
 */
#ifdef _WIN32
__declspec(dllexport)
#endif
void CRC_stateUpdate(void *in_pState, const void *in_pBuffer, size_t in_count);

#ifdef __cplusplus
}
#endif

#endif
