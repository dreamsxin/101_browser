#ifndef _CRC_h
#define _CRC_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_init();

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_update(uint32_t in_currentCRC, uint8_t in_currentByte);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_terminate(uint32_t in_currentCRC);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_foldl(uint32_t in_currentCRC, uint8_t *in_buffer, size_t in_bufferSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t CRC_compute(uint8_t *in_buffer, size_t in_bufferSize);

#ifdef __cplusplus
}
#endif

#endif