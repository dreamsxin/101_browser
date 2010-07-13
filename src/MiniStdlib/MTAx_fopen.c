#include "MiniStdlib/MTAx_fopen.h"

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
