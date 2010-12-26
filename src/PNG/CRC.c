#include "PNG/CRC.h"

const uint32_t cInitialCRC = 0xFFFFFFFF;
const uint32_t cCRC_polynomial = 0xedb88320;

uint32_t initCRC()
{
	return cInitialCRC;
}

uint32_t updateCRC(uint32_t in_currentCRC, uint8_t in_currentByte)
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

uint32_t terminateCRC(uint32_t in_currentCRC)
{
	return in_currentCRC ^ 0xFFFFFFFF;
}
