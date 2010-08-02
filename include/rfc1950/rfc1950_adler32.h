#ifndef _OZO_rfc1950_adler32_h
#define _OZO_rfc1950_adler32_h

/* 
 * gcc requires this header file for a definition of size_t
 */
#ifdef __GNUC__
# ifdef __cplusplus
#include <cstddef>
# else
#include <stddef.h>
# endif
#endif

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t rfc1950_adler32(void *in_buffer, size_t in_bufferSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t rfc1950_adler32_update(uint32_t in_adler, uint8_t *in_buffer, size_t in_bufferSize);

#ifdef _WIN32
__declspec(dllexport)
#endif
uint32_t rfc1950_adler32_fold_fun(uint32_t in_adler, uint8_t in_currentByte);

#ifdef __cplusplus
}
#endif

#endif
