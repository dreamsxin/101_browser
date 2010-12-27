#include "IO/fread.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/climits.h"
#include <assert.h>

size_t fread_withState(void *in_dstBuffer, size_t in_elementSize, size_t in_count, FILE* in_file,
	void *in_pState, void (*in_pHandler)(void* in_pState, const void* in_pBuffer, size_t in_count))
{
	size_t readCount;
	size_t countIndex;
	
	// Reason for this assertion: the loops won't terminate otherwise
	assert(in_count < SIZE_MAX);

	readCount = fread(in_dstBuffer, in_elementSize, in_count, in_file);

	for (countIndex = 0; countIndex < readCount; countIndex++)
	{
		(*in_pHandler)(in_pState, ((const uint8_t*) in_dstBuffer)+countIndex*in_elementSize, in_elementSize);
	}

	return readCount;
}
