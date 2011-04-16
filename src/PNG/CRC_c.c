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
#include "MiniStdlib/cstdbool.h"
#include <stdio.h>
#include <omp.h>

const uint32_t cInitialCRC = 0xFFFFFFFF;
/*
 * 5.5 Cyclic Redundancy Code algorithm
 * 
 * "The CRC polynomial employed is
 * x^32 + x^26 + x^23 + x^22 + x^16 + x^12 + x^11 + x^10 + x^8 + x^7 + x^5 + x^4 + x^2 + x + 1
 * In PNG, the 32-bit CRC is initialized to all 1's, and then the data from 
 * each byte is processed from the least significant bit (1) to the most 
 * significant bit (128)."
 * 
 * Note that the bits are written here in seemingly reverse order, because
 * "For the purpose of separating into bytes and ordering, the least significant 
 * bit of the 32-bit CRC is defined to be the coefficient of the x^31 term."
 */
const uint32_t cCRC_polynomial = 0xedb88320;

uint32_t CRC_init()
{
	return cInitialCRC;
}

uint32_t crc_table_entry(uint8_t in_index)
{
	static bool crcTableInitialized = false;
	static uint32_t crcTable[256];

	if (!crcTableInitialized)
	{
#pragma omp parallel
		{
			int index;
			uint8_t currentBitIndex;
			uint32_t crcValue;

#pragma omp for private(currentBitIndex, crcValue)
			for (index = 0; index < 256; index++)
			{
				crcValue = (uint32_t) index;

				for (currentBitIndex = 0; currentBitIndex < 8; currentBitIndex++)
				{
					// the least significant bit is the coefficient of the x^31 term
					if (crcValue & 1)
						crcValue = cCRC_polynomial ^ (crcValue >> 1);
					else
						crcValue >>= 1;
				}

				crcTable[index] = crcValue;
			}
		}

		crcTableInitialized = true;
	}

	return crcTable[in_index];
}

uint32_t CRC_update(uint32_t in_currentCRC, uint8_t in_currentByte)
{
	return crc_table_entry((in_currentCRC ^ in_currentByte) & 0xFF) ^ (in_currentCRC >> 8);
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
