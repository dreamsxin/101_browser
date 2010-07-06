#include "rfc1950/rfc1950.h"
#include "BasicDataStructures/HigherOrderFunctions/Fold.h"

uint32_t adler32_update(uint32_t in_adler, void *in_buffer, size_t in_bufferSize)
{
	return OZO::HigherOrderFunctions::foldl<uint32_t>(&adler32_fold_fun, 
		in_adler, in_buffer, in_bufferSize);
}
