#include "IO/FileByteStream.h"

size_t fileByteReadStreamRead(void *in_out_pFileByteStreamState, void *out_pBuffer, size_t in_count)
{
	FileByteStreamState *pFileByteStreamState = 
		(FileByteStreamState*) in_out_pFileByteStreamState;

	return fread(out_pBuffer, 1, in_count, pFileByteStreamState->mFile);
}

bool createFileByteReadStreamState(const char *in_filename,
	FileByteStreamState *in_pFileByteStreamState)
{
	in_pFileByteStreamState->mFile = MTAx_fopen(in_filename, "rb");

	return in_pFileByteStreamState->mFile != NULL;
}
