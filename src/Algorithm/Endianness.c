#include "Algorithm/Endianness.h"
#include <limits.h> // for SIZE_MAX
#include <assert.h>

void computeOppositeEndianness(const void * in_pData, void * out_pData, size_t in_bufferSize)
{
	uint8_t *l_pInData = (uint8_t*) in_pData;
	uint8_t *l_pOutData = (uint8_t*) out_pData;
	size_t i;

	assert(in_bufferSize < SIZE_MAX);

	for (i=0; i<in_bufferSize; i++)
	{
		l_pOutData[i] = l_pOutData[in_bufferSize-1-i];
	}
}

void flipEndianness(void * in_out_pData, size_t in_bufferSize)
{
	uint8_t *lData = (uint8_t*) in_out_pData;
	size_t i;

	for (i=0; i<in_bufferSize/2; i++)
	{
		uint8_t temp = lData[i];
		lData[i] = lData[in_bufferSize-1-i];
		lData[in_bufferSize-1-i] = temp;
	}
}
