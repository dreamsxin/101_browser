#include "MiniStdlib/MTAx_cstdlib.h"

FILE* MTAx_fopen(const char *in_cFilename, const char *in_cMode)
{
#ifdef _WIN32
	FILE* outFile;
	if (fopen_s(&outFile, in_cFilename, in_cMode) != 0)
		return NULL;
	else
		return outFile;
#else
	return fopen(in_cFilename, in_cMode);
#endif
}

int MTAx_fclose(FILE** in_ppFile)
{
	int result = fclose(*in_ppFile);
	*in_ppFile = NULL;
	return result;
}
