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
size_t fileByteStreamRead(void *in_out_pFileByteStreamState, void *in_pBuffer, size_t in_count);

#ifdef _WIN32
__declspec(dllexport)
#endif
bool createFileByteStreamState(const char *in_filename,
	FileByteStreamState *in_pFileByteStreamState);

const ByteStreamInterface cFileByteStreamInterface = { &fileByteStreamRead };

#ifdef __cplusplus
}
#endif

#endif
