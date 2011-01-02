#ifndef _MTAx_IO_ByteStreamInterface_h
#define _MTAx_IO_ByteStreamInterface_h

#include "MiniStdlib/cstdint.h"

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

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	size_t (*pRead)(void *in_out_pByteStreamState, void *out_pBuffer, size_t in_count);
} ByteReadStreamInterface;

typedef struct
{
	size_t (*pWrite)(void *in_out_pByteStreamState, const void *in_pBuffer, size_t in_count);
} ByteWriteStreamInterface;

#ifdef __cplusplus
}
#endif

#endif
