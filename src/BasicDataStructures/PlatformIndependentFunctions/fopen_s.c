#include "BasicDataStructures/PlatformIndependentFunctions/fopen_s.h"

errno_t pi_fopen_s(FILE ** in_ppFile, const char *in_cFilename, const char *in_cMode)
{
#ifdef _WIN32
	return fopen_s(in_ppFile, in_cFilename, in_cMode);
#else
	FILE* lFile = fopen(in_pFilename, in_cMode);

	if (lFile == 0)
		return -1;
	else
	{
		*in_ppFile = lFile;
		return 0;
	}
#endif
}
