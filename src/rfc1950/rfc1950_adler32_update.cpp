#include "rfc1950/rfc1950_adler32.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t rfc1950_adler32_update(uint32_t in_adler, uint8_t *in_buffer, size_t in_bufferSize)
{
	return OZO::HigherOrderFunctions::foldl<uint32_t, uint8_t>(&rfc1950_adler32_fold_fun, 
		in_adler, in_buffer, in_bufferSize);
}
