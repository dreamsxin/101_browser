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

#include "JpegDecoder/JpegSegmentsRead.h"
#include "MiniStdlib/memory.h"  // for ENDIANNESS_CONVERT_SIMPLE
#include "MiniStdlib/safe_free.h"
#include "SetjmpUtil/ConditionalLongjmp.h"
#include <assert.h>

void readRestartInterval(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	RestartIntervalState* in_pRestartIntervalState)
{
	SetjmpState invalidDataSetjmpState;
	
	(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, 
		&in_pRestartIntervalState->restartInterval, sizeof(in_pRestartIntervalState->restartInterval));

	in_pRestartIntervalState->isRestartIntervalInitialized = true;

	ENDIANNESS_CONVERT_SIMPLE(in_pRestartIntervalState->restartInterval.Lr);
	ENDIANNESS_CONVERT_SIMPLE(in_pRestartIntervalState->restartInterval.Ri);

	printf("Lr = %u\tRi = %u\n", 
		in_pRestartIntervalState->restartInterval.Lr, 
		in_pRestartIntervalState->restartInterval.Ri);

	setjmpStateInit(&invalidDataSetjmpState, 
		in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
		ReadResultInvalidData, printHandler);

	setjmpStateLongjmpIf(&invalidDataSetjmpState, 
		in_pRestartIntervalState->restartInterval.Lr != 4, 
		"Expected size 4 of DRI segment");
}

void readScanHeader(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	ScanHeader* in_pScanHeader)
{
	SetjmpState invalidDataSetjmpState;
	SetjmpState allocationFailureSetjmpState;

	uint8_t i;

	(*in_setjmpStreamReadInterface.mpfRead)(in_out_pSetjmpStreamState, 
		&in_pScanHeader->Ls, sizeof(in_pScanHeader->Ls)+sizeof(in_pScanHeader->Ns));

	ENDIANNESS_CONVERT_SIMPLE(in_pScanHeader->Ls);

	setjmpStateInit(&invalidDataSetjmpState, 
		in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
		ReadResultInvalidData, printHandler);

	// CND:JpegSegmentsRead_c_70
	setjmpStateLongjmpIf(&invalidDataSetjmpState, 
		in_pScanHeader->Ns == 0 || in_pScanHeader->Ns>4, 
		"readScanHeader: invalid value of Ns");
	setjmpStateLongjmpIf(&invalidDataSetjmpState, 
		in_pScanHeader->Ls != 6+2*in_pScanHeader->Ns, 
		"readScanHeader: Ls must be 6+2*Ns");

	printf("Ls = %u\tNs = %u\n", in_pScanHeader->Ls, in_pScanHeader->Ns);

	setjmpStateInit(&allocationFailureSetjmpState, 
		in_out_pSetjmpStreamState->setjmpState.mpJmpBuffer, 
		ReadResultAllocationFailure, NULL);

	// follows from CND:JpegSegmentsRead_c_70
	assert(in_pScanHeader->Ns <= 4);

	for (i=0; i<in_pScanHeader->Ns; i++)
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
