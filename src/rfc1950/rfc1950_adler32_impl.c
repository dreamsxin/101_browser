#include "rfc1950/rfc1950_adler32.h"

#define BASE 65521 /* largest prime smaller than 65536 */

uint32_t rfc1950_adler32(void *in_buffer, size_t in_bufferSize)
{
	return rfc1950_adler32_update(1ul, in_buffer, in_bufferSize);
}

uint32_t rfc1950_adler32_fold_fun(uint32_t in_adler, uint8_t in_currentByte)
{
	uint32_t s1 = in_adler & 0xffff;
	uint32_t s2 = (in_adler >> 16) & 0xffff;

	s1 = (s1 + in_currentByte) % BASE;
	s2 = (s2 + s1)             % BASE;

	return (s2 << 16) + s1;
}
