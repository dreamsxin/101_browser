#ifndef _OZO_rfc1950_h
#define _OZO_rfc1950_h

#include "BasicDataStructures/PlatformIndependentHeaders/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

__declspec(dllexport) uint32_t adler32(void *in_buffer, size_t in_bufferSize);
__declspec(dllexport) uint32_t adler32_update(
uint32_t in_adler, void *in_buffer, size_t in_bufferSize);

__declspec(dllexport) uint32_t adler32_fold_fun(uint32_t in_adler, uint8_t in_currentByte);

#ifdef __cplusplus
}
#endif

#endif
