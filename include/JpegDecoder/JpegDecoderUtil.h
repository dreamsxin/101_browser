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

#ifndef _JpegDecoderUtil
#define _JpegDecoderUtil

#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
#include "IO/SetjmpStream.h"

#ifdef __cplusplus
extern "C" {
#endif

bool isStandaloneMarker(unsigned char in_marker);
void printSegmentName(unsigned char in_marker);
void printMarkerInformation(unsigned char currentMarker);
unsigned char readMarker(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface);
void defaultMarkerInterpreter(SetjmpStreamState *in_out_pSetjmpStreamState, 
	ByteStreamInterface in_setjmpStreamReadInterface, 
	unsigned char currentMarker);

typedef enum _T81_EncodingProcess
{
	T81_EncodingProcess_InvalidParameter,
	T81_EncodingProcess_BaselineSequentialDCT,
	T81_EncodingProcess_ExtendedSequentialDCT,
	T81_EncodingProcess_ProgressiveDCT,
	T81_EncodingProcess_Lossless
} T81_EncodingProcess;

typedef enum _T81_Coding
{
	T81_Coding_InvalidParameter,
	T81_Coding_HuffmanCoding,
	T81_Coding_ArithmeticCoding
} T81_Coding;

/*!
* Returns the used encoding process. If a wrong parameter of SOF_n is passed, 
* T81_EncodingProcess_InvalidParameter is returned.
*/
T81_EncodingProcess getEncodingProcess(uint8_t SOF_n);

/*!
* Returns the used coding. If a wrong parameter of SOF_n is passed, 
* T81_Coding_InvalidParameter is returned.
*/
T81_Coding getCoding(uint8_t SOF_n);

#ifdef __cplusplus
}
#endif

#endif
