#include "JpegDecoder/JpegSegments.h"
#include <cstdlib>

void readRestartInterval(FILE* jpegFile, RestartInterval* in_pRestartInterval)
{
	if (fread(in_pRestartInterval, sizeof(RestartInterval), 1, jpegFile) != 1)
	{
		fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
		exit(1);
	}

	in_pRestartInterval->Lr = flipBytes(in_pRestartInterval->Lr);
	in_pRestartInterval->Ri = flipBytes(in_pRestartInterval->Ri);

	printf("Lr = %u\tRi = %u\n", in_pRestartInterval->Lr, in_pRestartInterval->Ri);

	if (in_pRestartInterval->Lr != 4)
	{
		fprintf(stderr, "Expected size 4 of DRI segment.\n");
		exit(1);
	}
}

void readScanHeader(FILE* jpegFile, ScanHeader* in_pScanHeader)
{
	if (fread(&in_pScanHeader->Ls, 
		sizeof(in_pScanHeader->Ls)+sizeof(in_pScanHeader->Ns), 1, jpegFile) != 1)
	{
		fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
		exit(1);
	}

	in_pScanHeader->Ls = flipBytes(in_pScanHeader->Ls);

	if (in_pScanHeader->Ns == 0 || in_pScanHeader->Ns>4)
	{
		fprintf(stderr, "Invalid value of Ns\n");
		exit(1);
	}

	if (in_pScanHeader->Ls != 6+2*in_pScanHeader->Ns)
	{
		fprintf(stderr, "Ls must be 6+2*Ns\n");
		exit(1);
	}

	printf("Ls = %u\tNs = %u\n", in_pScanHeader->Ls, in_pScanHeader->Ns);

	for (size_t i=0; i<in_pScanHeader->Ns; i++)
	{
		ScanHeader::ScanComponentSpecificationParameter scsp;
		if (fread(&scsp, sizeof(ScanHeader::ScanComponentSpecificationParameter), 
			1, jpegFile) != 1)
		{
			fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
			exit(1);
		}
		
		printf("Cs%u = %2X\t", i+1, scsp.Cs);
		printf("Td%u = %1X\t", i+1, scsp.Td);
		printf("Ta%u = %1X\n", i+1, scsp.Ta);

		in_pScanHeader->componentSpecificationParameters.push_back(scsp);
	}

	if (fread(&in_pScanHeader->Ss, 3, 1, jpegFile) != 1)
	{
		fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
		exit(1);
	}

	printf("Ss = %2X\tSe = %2X\tAh = %1X\tAl = %1X\n", 
		in_pScanHeader->Ss, in_pScanHeader->Se, in_pScanHeader->Ah, in_pScanHeader->Al);
}
