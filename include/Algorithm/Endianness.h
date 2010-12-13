#ifndef _Endianness_h
#define _Endianness_h

#include "MiniStdlib/cstdint.h"
 // for size_t
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

void computeOppositeEndianness(const void * in_pData, void * out_pData, size_t in_bufferSize);
void flipEndianness(void * in_out_pData, size_t in_bufferSize);

#ifdef __cplusplus
}
#endif

#endif
