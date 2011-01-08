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

#include "PNG/CRC.h"

const uint32_t cInitialCRC = 0xFFFFFFFF;
const uint32_t cCRC_polynomial = 0xedb88320;

uint32_t CRC_init()
{
	return cInitialCRC;
}

uint32_t CRC_update(uint32_t in_currentCRC, uint8_t in_currentByte)
{
	uint8_t currentBitIdx;
	uint32_t byteCRC = (in_currentCRC ^ in_currentByte) & 0xFF;

	for (currentBitIdx = 0; currentBitIdx < 8; ++currentBitIdx)
	{
		if (byteCRC & 1)
		{
			byteCRC = cCRC_polynomial ^ (byteCRC >> 1);
		}
		else
		{
			byteCRC >>= 1;
		}
	}

	return (in_currentCRC >> 8) ^ byteCRC;
}

uint32_t CRC_terminate(uint32_t in_currentCRC)
{
	return in_currentCRC ^ 0xFFFFFFFF;
}

uint32_t CRC_compute(uint8_t *in_buffer, size_t in_bufferSize)
{
	return CRC_terminate(CRC_foldl(CRC_init(), in_buffer, in_bufferSize));
}

void CRC_stateUpdate(void *in_pState, const void *in_pBuffer, size_t in_count)
{
	uint32_t *pState = (uint32_t*) in_pState;
	*pState = CRC_foldl(*pState, (uint8_t*) in_pBuffer, in_count);
}
