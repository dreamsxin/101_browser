#ifndef _MTAx_Algorithm_Bitmask_h
#define _MTAx_Algorithm_Bitmask_h

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

bool checkBitmask(const void *in_pBuffer, size_t in_bufferSize, const void *in_pMaskIdentifierBits, const void *in_pMaskValueBits);

#ifdef __cplusplus
}
#endif

#endif
