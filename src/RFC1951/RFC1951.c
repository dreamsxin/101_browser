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

static ReadResult notImplemented(BitReadState *in_pBitReadState) { return ReadResultNotImplemented; }
static ReadResult invalidData(BitReadState *in_pBitReadState) { return ReadResultInvalidData; }

ReadResult readDataNoCompression(BitReadState *in_pBitReadState)
{
	uint16_t len, nlen;

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


	return ReadResultOK;
}

ReadResult readDataCompressedWithDynamicHuffmanCodes(BitReadState *in_pBitReadState)
{
	uint16_t literalCodesCount = 0;
	uint8_t distanceCodesCount, codeLengthCodesCount;

	if (readBitsLittleEndian(in_pBitReadState, &literalCodesCount, 5) != 5)
		return ReadResultPrematureEndOfStream;
	literalCodesCount += 257;

	if (literalCodesCount > 286)
		return ReadResultInvalidData;

	if (readBitsLittleEndian(in_pBitReadState, &distanceCodesCount, 5) != 5)
		return ReadResultPrematureEndOfStream;
	distanceCodesCount += 1;

	if (distanceCodesCount > 32) // perhaps 30?
		return ReadResultInvalidData;

	if (readBitsLittleEndian(in_pBitReadState, &codeLengthCodesCount, 4) != 4)
		return ReadResultPrematureEndOfStream;
	codeLengthCodesCount += 4;

	
	
	


	return ReadResultOK;
}

ReadResult (*pfTypeFunctions[4])(BitReadState *in_pBitReadState) = {
	readDataNoCompression, 
	notImplemented, 
	readDataCompressedWithDynamicHuffmanCodes,
	invalidData
};

ReadResult parseRFC1951(void *in_out_pReadStreamState, ByteStreamReadInterface in_readInterface, 
	void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	BitReadState bitReadState;
	uint8_t last, type;
	ReadResult readResult;

	initBitReadState(&bitReadState, in_out_pReadStreamState, in_readInterface);

	do
	{
		if (readBitsLittleEndian(&bitReadState, &last, 1) != 1)
			return ReadResultPrematureEndOfStream;
		if (readBitsLittleEndian(&bitReadState, &type, 2) != 2)
			return ReadResultPrematureEndOfStream;

		readResult = pfTypeFunctions[type](&bitReadState);

		if (readResult != ReadResultOK)
			return readResult;
	} while (!last);

	return ReadResultOK;
}
