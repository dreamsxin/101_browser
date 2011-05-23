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

#include "MiniStdlib/memset.h"
#include "RFC1951/RFC1951.h"
#include "IO/BitRead.h"
#include "IO/SetjmpStream.h"
#include <string.h>

typedef struct
{
	uint16_t *count;       /* number of symbols of each length */
    uint16_t *symbol;      /* canonically ordered symbols */
} HuffmanNode;

#define MAXIMUM_BITS_IN_A_CODE 15

/*
* Preconditions:
* PRE:RFC1951_c_32: in_pNode->count has a size of at least MAXIMUM_BITS_IN_A_CODE+1
* PRE:RFC1951_c_33: 0 <= in_pLengths[i] <= MAXBITS for all i in 0..in_lengthsCount
*/
void constructHuffmanTree(HuffmanNode *in_pNode, uint16_t *in_pLengths, size_t in_lengthsCount)
{
	size_t currentLength, currentSymbol;

	// No buffer overflow because of PRE:RFC1951_c_32
	memset(in_pNode->count, 0, sizeof(uint16_t)*(MAXIMUM_BITS_IN_A_CODE+1));

	for (currentSymbol = 0; currentSymbol < in_lengthsCount; currentSymbol++)
		// No buffer overflow because of PRE:RFC1951_c_33
		(in_pNode->count[in_pLengths[currentSymbol]])++;


}

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

	/*
	* Q: Why is this line very important?
	* A: If it wasn't here we would get a problem when there is a block after 
	*    this uncompressed block (we would read data from the buffer instead of
	*    new data).
	*/
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

#define FIXED_LITERALLENGTH_HUFFMAN_CODES_COUNT 288
#define MAXIMUM_NUMBER_OF_LITERAL_LENGTH_CODES 286           /* maximum number of literal/length codes */
#define MAXIMUM_NUMBER_OF_DISTANCE_CODES 30

void readDataCompressedWithFixedHuffmanCodes(BitReadState *in_pBitReadState, 
	SetjmpWriteStreamState *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	static bool tableInitialized = false;
	HuffmanNode lengthCode, distanceCode;
	static uint16_t lencnt[MAXIMUM_BITS_IN_A_CODE+1], lensym[FIXED_LITERALLENGTH_HUFFMAN_CODES_COUNT];
    static uint16_t distcnt[MAXIMUM_BITS_IN_A_CODE+1], distsym[MAXIMUM_NUMBER_OF_DISTANCE_CODES];

	if (!tableInitialized)
	{
		uint16_t codeLengths[FIXED_LITERALLENGTH_HUFFMAN_CODES_COUNT];

		{
			const uint16_t literalValueRangeBoundaries[LITERAL_VALUE_RANGE_BOUNDARY_COUNT+1] = 
			{ 0, 144, 256, 280, FIXED_LITERALLENGTH_HUFFMAN_CODES_COUNT };
			const uint8_t literalCodeLengthsInArea[LITERAL_VALUE_RANGE_BOUNDARY_COUNT] = { 8, 9, 7, 8 };
			uint8_t idx;

			for (idx = 0; idx < LITERAL_VALUE_RANGE_BOUNDARY_COUNT; idx++)
			{
				memset2(codeLengths+literalValueRangeBoundaries[idx], literalCodeLengthsInArea[idx], 
					literalValueRangeBoundaries[idx+1]-literalValueRangeBoundaries[idx]);
			}
		}
		lengthCode.count = lencnt;
		lengthCode.symbol = lensym;
		constructHuffmanTree(&lengthCode, codeLengths, FIXED_LITERALLENGTH_HUFFMAN_CODES_COUNT);

		memset2(codeLengths, 5, MAXIMUM_NUMBER_OF_DISTANCE_CODES);
		distanceCode.count = distcnt;
		distanceCode.symbol = distsym;
		constructHuffmanTree(&distanceCode, codeLengths, MAXIMUM_NUMBER_OF_DISTANCE_CODES);
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
