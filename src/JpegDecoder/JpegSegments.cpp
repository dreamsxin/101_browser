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

void readRestartInterval(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	RestartInterval* in_pRestartInterval)
{
	(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, in_pRestartInterval, sizeof(*in_pRestartInterval));

	ENDIANNESS_CONVERT_SIMPLE(in_pRestartInterval->Lr);
	ENDIANNESS_CONVERT_SIMPLE(in_pRestartInterval->Ri);

	printf("Lr = %u\tRi = %u\n", in_pRestartInterval->Lr, in_pRestartInterval->Ri);

	if (in_pRestartInterval->Lr != 4)
	{
		fprintf(stderr, "Expected size 4 of DRI segment.\n");
		exit(1);
	}
}

void readScanHeader(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	ScanHeader* in_pScanHeader)
{
	(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, 
		&in_pScanHeader->Ls, sizeof(in_pScanHeader->Ls)+sizeof(in_pScanHeader->Ns));

	ENDIANNESS_CONVERT_SIMPLE(in_pScanHeader->Ls);

	if (in_pScanHeader->Ns == 0 || in_pScanHeader->Ns>4)
	{
		fprintf(stderr, "readScanHeader: invalid value of Ns\n");
		longjmp(*in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, ReadResultInvalidData);
	}

	if (in_pScanHeader->Ls != 6+2*in_pScanHeader->Ns)
	{
		fprintf(stderr, "readScanHeader: Ls must be 6+2*Ns\n");
		longjmp(*in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, ReadResultInvalidData);
	}

	printf("Ls = %u\tNs = %u\n", in_pScanHeader->Ls, in_pScanHeader->Ns);

	in_pScanHeader->componentSpecificationParameters = NULL;

	in_pScanHeader->componentSpecificationParameters = (ScanComponentSpecificationParameter *) 
		malloc(sizeof(ScanComponentSpecificationParameter) * in_pScanHeader->Ns);

	if (!in_pScanHeader->componentSpecificationParameters)
	{
		longjmp(*in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, ReadResultAllocationFailure);
	}

	for (size_t i=0; i<in_pScanHeader->Ns; i++)
	{
		(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, 
			in_pScanHeader->componentSpecificationParameters+i, 
			sizeof(ScanComponentSpecificationParameter));
		
		printf("Cs%u = %2X\t", i+1, in_pScanHeader->componentSpecificationParameters[i].Cs);
		printf("Td%u = %1X\t", i+1, in_pScanHeader->componentSpecificationParameters[i].Td);
		printf("Ta%u = %1X\n", i+1, in_pScanHeader->componentSpecificationParameters[i].Ta);
	}

	(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, 
		&in_pScanHeader->Ss, sizeof(in_pScanHeader->Ss));

	printf("Ss = %2X\tSe = %2X\tAh = %1X\tAl = %1X\n", 
		in_pScanHeader->Ss, in_pScanHeader->Se, in_pScanHeader->Ah, in_pScanHeader->Al);
}
