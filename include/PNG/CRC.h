#ifndef _CRC_h
#define _CRC_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t initCRC();

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t updateCRC(uint32_t in_currentCRC, uint8_t in_currentByte);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t terminateCRC(uint32_t in_currentCRC);

#ifdef __cplusplus
}
#endif

#endif