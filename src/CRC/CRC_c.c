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

#include "CRC/CRC.h"
#include "MiniStdlib/cassert.h"

const uint32_t cCRC_polynomials[] = {
	0xedb88320,
	0x04c11db7
};

static uint32_t crcTables[2][256];

void initCRC_Tables()
{
	int index;
	uint8_t currentBitIndex;
	uint32_t crcValue;

	// CRC_BitOrder_LSB_to_MSB
	for (index = 0; index < 256; index++)
	{
		crcValue = (uint32_t) index;

		for (currentBitIndex = 0; currentBitIndex < 8; currentBitIndex++)
		{
			// the least significant bit is the coefficient of the x^31 term
			if (crcValue & 1)
				crcValue = cCRC_polynomials[CRC_BitOrder_LSB_to_MSB] ^ (crcValue >> 1);
			else
				crcValue >>= 1;
		}

		crcTables[CRC_BitOrder_LSB_to_MSB][index] = crcValue;
	}

	// CRC_BitOrder_MSB_to_LSB
	for (index = 0; index < 256; index++)
	{
		crcValue = ((uint32_t) index) << 24;

		for (currentBitIndex = 0; currentBitIndex < 8; currentBitIndex++)
		{
			// the least significant bit is the coefficient of the x^31 term
			if (crcValue & 0x80000000UL)
				crcValue = cCRC_polynomials[CRC_BitOrder_MSB_to_LSB] ^ (crcValue << 1);
			else
				crcValue <<= 1;
		}

		crcTables[CRC_BitOrder_MSB_to_LSB][index] = crcValue;
	}
}

static uint32_t initializeCrcTableAndReturnInitialCrc(bool in_invertBefore);
static uint32_t initialCrc();

uint32_t (*crcInitFun)(bool) = initializeCrcTableAndReturnInitialCrc;

static uint32_t initializeCrcTableAndReturnInitialCrc(bool in_invertBefore)
{
	initCRC_Tables();
	crcInitFun = initialCrc;
	return initialCrc();
}

static uint32_t initialCrc(bool in_invertBefore)
{
	if (in_invertBefore)
		return 0xFFFFFFFF;
	else
		return 0x0;
}

uint32_t CRC_init(bool in_invertBefore)
{
	return (*crcInitFun)(in_invertBefore);
}

uint32_t CRC_update_LSB_to_MSB(uint32_t in_currentCRC, uint8_t in_currentByte)
{
	return crcTables[CRC_BitOrder_LSB_to_MSB][(in_currentCRC ^ in_currentByte) & 0xFF] ^ (in_currentCRC >> 8);
}

uint32_t CRC_update_MSB_to_LSB(uint32_t in_currentCRC, uint8_t in_currentByte)
{
	return crcTables[CRC_BitOrder_MSB_to_LSB][(in_currentCRC >> 24) ^ in_currentByte] ^ (in_currentCRC << 8);
}

uint32_t CRC_terminate(uint32_t in_currentCRC, bool in_invertAfter)
{
	if (in_invertAfter)
		return in_currentCRC ^ 0xFFFFFFFF;
	else
		return in_currentCRC;
}

uint32_t CRC_compute_LSB_TO_MSB(uint8_t *in_buffer, size_t in_bufferSize, 
	bool in_invertBefore, bool in_invertAfter)
{
	return CRC_terminate(CRC_foldl_LSB_TO_MSB(CRC_init(in_invertBefore), 
		in_buffer, in_bufferSize), in_invertAfter);
}

uint32_t CRC_compute_MSB_TO_LSB(uint8_t *in_buffer, size_t in_bufferSize, 
	bool in_invertBefore, bool in_invertAfter)
{
	return CRC_terminate(CRC_foldl_MSB_TO_LSB(CRC_init(in_invertBefore), 
		in_buffer, in_bufferSize), in_invertAfter);
}

void CRC_stateUpdate_update_LSB_to_MSB(void *in_pState, const void *in_pBuffer, size_t in_count)
{
	uint32_t *pState = (uint32_t*) in_pState;
	*pState = CRC_foldl_LSB_TO_MSB(*pState, (uint8_t*) in_pBuffer, in_count);
}

void CRC_stateUpdate_update_MSB_to_LSB(void *in_pState, const void *in_pBuffer, size_t in_count)
{
	uint32_t *pState = (uint32_t*) in_pState;
	*pState = CRC_foldl_MSB_TO_LSB(*pState, (uint8_t*) in_pBuffer, in_count);
}
