#ifndef _MTAx_IO_FileByteStream_h
#define _MTAx_IO_FileByteStream_h

#include "IO/ByteStreamInterface.h"
#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	FILE *mFile;
} FileByteStreamState;

#ifdef _WIN32
__declspec(dllexport)
#endif
size_t fileByteReadStreamRead(void *in_out_pFileByteStreamState, void *out_pBuffer, size_t in_count);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool createFileByteReadStreamState(const char *in_filename,
	FileByteStreamState *in_pFileByteStreamState);

const ByteReadStreamInterface cFileByteStreamInterface = { &fileByteReadStreamRead };

#ifdef __cplusplus
}
#endif

#endif
