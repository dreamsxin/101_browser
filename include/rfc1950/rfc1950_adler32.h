#ifndef _OZO_rfc1950_adler32_h
#define _OZO_rfc1950_adler32_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) uint32_t rfc1950_adler32(void *in_buffer, size_t in_bufferSize);
__declspec(dllexport) uint32_t rfc1950_adler32_update(
	uint32_t in_adler, void *in_buffer, size_t in_bufferSize);

__declspec(dllexport) uint32_t rfc1950_adler32_fold_fun(uint32_t in_adler, uint8_t in_currentByte);

#ifdef __cplusplus
}
#endif

#endif
