/*
* Copyright 2011 Wolfgang Keller
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

#include "RFC1951/RFC1951.h"
#include "IO/BitRead.h"
#include "IO/SetjmpStream.h"
#include <string.h>

static void invalidData(BitReadState *in_pBitReadState, 
	SetjmpWriteStreamState *in_out_pWriteStreamState, 
	ByteStreamWriteInterface in_writeInterface)
{
	longjmp(*in_out_pWriteStreamState->utilData.mpJmpBuffer, ReadResultInvalidData);
}

void readDataNoCompression(BitReadState *in_pBitReadState, 
	SetjmpWriteStreamState *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	uint16_t len, nlen, currentByteIndex;
	uint8_t currentByte;

	bitReadStateFlush(in_pBitReadState);

	/*
	* 3.2.4. Non-compressed blocks (BTYPE=00)
	*
	* Any bits of input up to the next byte boundary are ignored.
	* The rest of the block consists of the following information:
	* 
	*      0   1   2   3   4...
	*    +---+---+---+---+================================+
	*    |  LEN  | NLEN  |... LEN bytes of literal data...|
	*    +---+---+---+---+================================+
	* 
	* LEN is the number of data bytes in the block.  NLEN is the
	* one's complement of LEN.
	*/

	// Pay attention: the following lines only work on little-endian processors
	(*in_pBitReadState->readInterface.pRead)(in_pBitReadState->pByteStreamState, &len, sizeof(len));
	(*in_pBitReadState->readInterface.pRead)(in_pBitReadState->pByteStreamState, &nlen, sizeof(nlen));

	if (len != (uint16_t) ~nlen)
		longjmp(*in_out_pWriteStreamState->utilData.mpJmpBuffer, ReadResultInvalidData);

	for (currentByteIndex = 0; currentByteIndex < len; currentByteIndex++)
	{
		(*in_pBitReadState->readInterface.pRead)(in_pBitReadState->pByteStreamState, &currentByte, 1);
		(*in_writeInterface.pWrite)(in_out_pWriteStreamState, &currentByte, 1);
	}
}

#define LITERAL_VALUE_RANGE_BOUNDARY_COUNT 4
#define FIXED_HUFFMAN_CODE_LENGTH_COUNT 288

void readDataCompressedWithFixedHuffmanCodes(BitReadState *in_pBitReadState, 
	SetjmpWriteStreamState *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	static bool tableInitialized = false;

	if (!tableInitialized)
	{
		int literalValueRangeBoundaries[LITERAL_VALUE_RANGE_BOUNDARY_COUNT+1] = 
		{ 0, 144, 256, 280, FIXED_HUFFMAN_CODE_LENGTH_COUNT };
		uint8_t literalCodeLengthsInArea[LITERAL_VALUE_RANGE_BOUNDARY_COUNT] = { 8, 9, 7, 8 };
		uint8_t codeLengths[FIXED_HUFFMAN_CODE_LENGTH_COUNT];
		int idx;

		for (idx = 0; idx < LITERAL_VALUE_RANGE_BOUNDARY_COUNT; idx++)
		{
			memset(codeLengths+literalValueRangeBoundaries[idx], literalCodeLengthsInArea[idx], 
				literalValueRangeBoundaries[idx+1]-literalValueRangeBoundaries[idx]);
		}

		// ...

		memset(codeLengths, 5, FIXED_HUFFMAN_CODE_LENGTH_COUNT);

		// ...
	}


	// ...
}

void readDataCompressedWithDynamicHuffmanCodes(BitReadState *in_pBitReadState, 
	SetjmpWriteStreamState *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	uint16_t literalCodesCount = 0;
	uint8_t distanceCodesCount, codeLengthCodesCount;

	readBitsLittleEndian(in_pBitReadState, &literalCodesCount, 5);
	literalCodesCount += 257;

	/*
	* 5 Bits: HLIT, # of Literal/Length codes - 257 (257 - 286)
	*/
	if (literalCodesCount > 286)
		longjmp(*in_out_pWriteStreamState->utilData.mpJmpBuffer, ReadResultInvalidData);

	readBitsLittleEndian(in_pBitReadState, &distanceCodesCount, 5);
	distanceCodesCount += 1;

	/*
	* Open question: why does the puff code check whether we have more than
	* 30 distance codes while RFC 1951 says in 
	* chapter "3.2.7. Compression with dynamic Huffman codes (BTYPE=10)":
	* "5 Bits: HDIST, # of Distance codes - 1        (1 - 32)"
	*
	* Quote from puff.c
	* if (nlen > MAXLCODES || ndist > MAXDCODES)
	*     return -3; 
	*
	* (where MAXDCODES equals 30)
	*/
#if 0
	if (distanceCodesCount > 30)
		return ReadResultInvalidData;
#endif

	readBitsLittleEndian(in_pBitReadState, &codeLengthCodesCount, 4);
	codeLengthCodesCount += 4;

	// ...
}

void (*pfTypeFunctions[4])(BitReadState *in_pBitReadState, 
	SetjmpWriteStreamState *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface) = {
		readDataNoCompression, 
		readDataCompressedWithFixedHuffmanCodes, 
		readDataCompressedWithDynamicHuffmanCodes,
		invalidData
};

ReadResult parseRFC1951(void *in_out_pReadStreamState, ByteStreamReadInterface in_readInterface, 
	void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	BitReadState bitReadState;

	SetjmpReadStreamState setjmpReadStreamState;
	SetjmpWriteStreamState setjmpWriteStreamState;

	jmp_buf jmpBuf;
	int result;

	uint8_t last, type;
	ReadResult readResult;

	setjmpReadStreamInit(&setjmpReadStreamState, &jmpBuf, ReadResultPrematureEndOfStream, 
		in_out_pReadStreamState, in_readInterface);

	setjmpWriteStreamInit(&setjmpWriteStreamState, &jmpBuf, ReadResultWriteError, 
		in_out_pWriteStreamState, in_writeInterface);

	// The = is correct here
	if (result = setjmp(jmpBuf))
		return (ReadResult) result;

	initBitReadState(&bitReadState, &setjmpReadStreamState, getSetjmpReadStreamReadInterface());

	do
	{
		readBitsLittleEndian(&bitReadState, &last, 1);
		readBitsLittleEndian(&bitReadState, &type, 2);

		pfTypeFunctions[type](&bitReadState, &setjmpWriteStreamState, getSetjmpWriteStreamReadInterface());
	} while (!last);

	return ReadResultOK;
}
