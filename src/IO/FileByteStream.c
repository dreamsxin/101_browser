#include "IO/FileByteStream.h"

size_t fileByteStreamRead(void *in_out_pFileByteStreamState, void *in_pBuffer, 
	size_t in_count, void *in_pAdditionalData)
{
	FileByteStreamState *pFileByteStreamState = 
		(FileByteStreamState*) in_out_pFileByteStreamState;

	return fread(in_pBuffer, 1, in_count, pFileByteStreamState->mFile);
}

bool createFileByteStreamState(const char *in_filename,
	FileByteStreamState *in_pFileByteStreamState)
{
	in_pFileByteStreamState->mFile = MTAx_fopen(in_filename, "rb");

	return in_pFileByteStreamState->mFile != NULL;
}
