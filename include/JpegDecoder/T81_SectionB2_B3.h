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

#ifndef _T81_SectionB2_B3
#define _T81_SectionB2_B3

#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdint.h"
#include "JpegDecoder/JpegDecoderUtil.h"
#include "Util/ReadResult.h"



#pragma pack(push, 1)

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4214) // nonstandard extension used : bit field types other than int
#endif

// B.2.2 Frame header syntax
typedef struct _FrameComponentSpecificationParameter
{
	uint8_t C;
	/*
	* Note that the order is changed from the specification since
	* the specification uses big-endian bit ordering while X86 compilers
	* use little-endian
	*/
	uint8_t V : 4;
	uint8_t H : 4;
	uint8_t Tq;
} FrameComponentSpecificationParameter;

typedef struct _FrameHeader
{
	uint8_t SOF_n;
	uint16_t Lf;
	uint8_t P;
	uint16_t Y;
	uint16_t X;
	uint8_t Nf;
	FrameComponentSpecificationParameter *pFrameComponentSpecificationParameters;
} FrameHeader;

// B.2.3 Scan header syntax
typedef struct _ScanComponentSpecificationParameter
{
	uint8_t Cs;
	/*
	* Note that the order is changed from the specification since
	* the specification uses big-endian bit ordering while X86 compilers
	* use little-endian
	*/
	uint8_t Ta : 4;
	uint8_t Td : 4;
} ScanComponentSpecificationParameter;

typedef struct _ScanHeader
{
	uint16_t Ls; // Scan header length � Specifies the length of the 
	             // scan header shown in Figure B.4 (see B.1.1.4).
	uint8_t Ns;  // Number of image components in scan � Specifies 
	             // the number of source image components in the scan. 
	             // The value of Ns shall be equal to the number of sets
	             // of scan component specification parameters 
	             // (Csj, Tdj, and Taj) present in the scan header.

	/*
	* Ns tells us how how many of the componentSpecificationParameters
	* are actually used
	*/
	ScanComponentSpecificationParameter componentSpecificationParameters[4];

	uint8_t Ss;
	uint8_t Se;
	/*
	* Note that the order is changed from the specification since
	* the specification uses big-endian bit ordering while X86 compilers
	* use little-endian
	*/
	uint8_t Al : 4;
	uint8_t Ah : 4;
} ScanHeader;

// B.2.4.1 Quantization table-specification syntax
typedef struct _QuantizationTable
{
	/*
	* Note that the order is changed from the specification since
	* the specification uses big-endian bit ordering while X86 compilers
	* use little-endian
	*/
	uint8_t Tq : 4;
	uint8_t Pq : 4;
	union
	{
		uint8_t Q8[64];
		uint16_t Q16[64];
	} Q;
} QuantizationTable;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// B.2.4.4 Restart interval definition syntax
typedef struct _RestartInterval
{
	uint16_t Lr; // Define restart interval segment length
	uint16_t Ri; // Specifies the number of MCU in the restart interval.
} RestartInterval;

#pragma pack(pop)

#endif
