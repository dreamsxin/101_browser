#include "Algorithm/Bitmask.h"
#include "MiniStdlib/cstddef.h" // for SIZE_MAX on Unix
#include <assert.h>

bool checkBitmask(void *in_pBuffer, size_t in_bufferSize, const void *in_pMaskIdentifierBits, const void *in_pMaskValueBits)
{
	const uint8_t *pBuffer = (const uint8_t*) in_pBuffer;
	const uint8_t *pMaskIdentifierBits = (const uint8_t*) in_pMaskIdentifierBits;
	const uint8_t *pMaskValueBits = (const uint8_t*) in_pMaskValueBits;
	size_t index;
	uint8_t currentBit;
	
	assert(in_bufferSize < SIZE_MAX);

	for (index = 0; index < in_bufferSize; index++)
	{
		for (currentBit = 0; currentBit < 8; currentBit++)
		{
			if (pMaskIdentifierBits[index] & (1 << currentBit))
			{
				if ((pBuffer[index] & (1 << currentBit)) != (pMaskValueBits[index] & (1 << currentBit)))
				{
					return false;
				}
			}
		}
	}

	return true;
}
