#include "RFC1950/RFC1950_adler32.h"

#define BASE 65521 /* largest prime smaller than 65536 */

uint32_t RFC1950_adler32_init()
{
	return 1;
}

uint32_t RFC1950_adler32_compute(void *in_buffer, size_t in_bufferSize)
{
	return RFC1950_adler32_foldl(RFC1950_adler32_init(), (uint8_t*) in_buffer, in_bufferSize);
}

uint32_t RFC1950_adler32_update(uint32_t in_adler, uint8_t in_currentByte)
{
	uint32_t s1 = in_adler & 0xffff;
	uint32_t s2 = (in_adler >> 16) & 0xffff;

	s1 = (s1 + in_currentByte) % BASE;
	s2 = (s2 + s1)             % BASE;

	return (s2 << 16) + s1;
}
