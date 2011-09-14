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
#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/declspec.h"
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
	CRC_BitOrder_LSB_to_MSB,
	CRC_BitOrder_MSB_to_LSB
} CRC_BitOrder;

DLLEXPORT uint32_t CRC_init(bool in_invertBefore);

/*!
* Precondition: the function CRC_init() was called before.
*/
DLLEXPORT uint32_t CRC_update_LSB_to_MSB(uint32_t in_currentCRC, uint8_t in_currentByte);
DLLEXPORT uint32_t CRC_update_MSB_to_LSB(uint32_t in_currentCRC, uint8_t in_currentByte);

DLLEXPORT uint32_t CRC_terminate(uint32_t in_currentCRC, bool in_invertAfter);

/*!
* Precondition: the function CRC_init() was called before.
*/
DLLEXPORT uint32_t CRC_foldl_LSB_TO_MSB(uint32_t in_currentCRC, uint8_t *in_buffer, size_t in_bufferSize);
DLLEXPORT uint32_t CRC_foldl_MSB_TO_LSB(uint32_t in_currentCRC, uint8_t *in_buffer, size_t in_bufferSize);

DLLEXPORT uint32_t CRC_compute_LSB_TO_MSB(uint8_t *in_buffer, size_t in_bufferSize, 
	bool in_invertBefore, bool in_invertAfter);
DLLEXPORT uint32_t CRC_compute_MSB_TO_LSB(uint8_t *in_buffer, size_t in_bufferSize, 
	bool in_invertBefore, bool in_invertAfter);

/*!
 * The function CRC_stateUpdate is for using with fread_withState (IO/fread.h).
 * Precondition: the function CRC_init() was called before.
 */
DLLEXPORT void CRC_stateUpdate_update_LSB_to_MSB(void *in_pState, const void *in_pBuffer, size_t in_count);
DLLEXPORT void CRC_stateUpdate_update_MSB_to_LSB(void *in_pState, const void *in_pBuffer, size_t in_count);

#ifdef __cplusplus
}
#endif

#endif
