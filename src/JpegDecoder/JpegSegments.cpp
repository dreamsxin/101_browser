/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "JpegDecoder/JpegSegments.h"
#include "MiniStdlib/memory.h"  // for ENDIANNESS_CONVERT_SIMPLE
#include <cstdlib>

void readRestartInterval(FILE* jpegFile, RestartInterval* in_pRestartInterval)
{
	if (fread(in_pRestartInterval, sizeof(RestartInterval), 1, jpegFile) != 1)
	{
		fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
		exit(1);
	}

	ENDIANNESS_CONVERT_SIMPLE(in_pRestartInterval->Lr);
	ENDIANNESS_CONVERT_SIMPLE(in_pRestartInterval->Ri);

	printf("Lr = %u\tRi = %u\n", in_pRestartInterval->Lr, in_pRestartInterval->Ri);

	if (in_pRestartInterval->Lr != 4)
	{
		fprintf(stderr, "Expected size 4 of DRI segment.\n");
		exit(1);
	}
}

ReadResult readScanHeader(FILE* jpegFile, ScanHeader* in_pScanHeader)
{
	if (fread(&in_pScanHeader->Ls, 
		sizeof(in_pScanHeader->Ls)+sizeof(in_pScanHeader->Ns), 1, jpegFile) != 1)
	{
		fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
		exit(1);
	}

	ENDIANNESS_CONVERT_SIMPLE(in_pScanHeader->Ls);

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

	in_pScanHeader->componentSpecificationParameters = (ScanComponentSpecificationParameter *) 
		malloc(sizeof(ScanComponentSpecificationParameter) * in_pScanHeader->Ns);

	if (!in_pScanHeader->componentSpecificationParameters)
		return ReadResultAllocationFailure;

	for (size_t i=0; i<in_pScanHeader->Ns; i++)
	{
		ScanComponentSpecificationParameter scsp;
		if (fread(in_pScanHeader->componentSpecificationParameters+i, sizeof(ScanComponentSpecificationParameter), 
			1, jpegFile) != 1)
		{
			fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
			exit(1);
		}
		
		printf("Cs%u = %2X\t", i+1, in_pScanHeader->componentSpecificationParameters[i].Cs);
		printf("Td%u = %1X\t", i+1, in_pScanHeader->componentSpecificationParameters[i].Td);
		printf("Ta%u = %1X\n", i+1, in_pScanHeader->componentSpecificationParameters[i].Ta);
	}

	if (fread(&in_pScanHeader->Ss, 3, 1, jpegFile) != 1)
	{
		fprintf(stderr, "In readRestartInterval: preliminary end of file\n");
		exit(1);
	}

	printf("Ss = %2X\tSe = %2X\tAh = %1X\tAl = %1X\n", 
		in_pScanHeader->Ss, in_pScanHeader->Se, in_pScanHeader->Ah, in_pScanHeader->Al);
}
