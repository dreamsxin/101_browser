#include "JpegDecoder/JpegSegments.h"
#include <cstdlib>

void readRestartInterval(FILE* jpegFile, RestartInterval* in_pRestartInterval)
{
	fread(in_pRestartInterval, sizeof(RestartInterval), 1, jpegFile);

	in_pRestartInterval->Lr = flipBytes(in_pRestartInterval->Lr);
	in_pRestartInterval->Ri = flipBytes(in_pRestartInterval->Ri);

	printf("Lr = %u\tRi = %u\n", in_pRestartInterval->Lr, in_pRestartInterval->Ri);

	if (in_pRestartInterval->Lr != 4)
	{
		fprintf(stderr, "Expected size 4 of DRI segment.\n");
		exit(1);
	}
}
