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

static ReadResult notImplemented(BitReadState *in_pBitReadState, void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface) { return ReadResultNotImplemented; }
static ReadResult invalidData(BitReadState *in_pBitReadState, void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface) { return ReadResultInvalidData; }

ReadResult readDataNoCompression(BitReadState *in_pBitReadState, 
	void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
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
	{
		return ReadResultInvalidData;
	}

	for (currentByteIndex = 0; currentByteIndex < len; currentByteIndex++)
	{
		(*in_pBitReadState->readInterface.pRead)(in_pBitReadState->pByteStreamState, &currentByte, sizeof(currentByte));
		
		if ((*in_writeInterface.pWrite)(in_out_pWriteStreamState, &currentByte, 1) != 1)
			return ReadResultWriteError;
	}

	return ReadResultOK;
}

ReadResult readDataCompressedWithDynamicHuffmanCodes(BitReadState *in_pBitReadState, 
	void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	uint16_t literalCodesCount = 0;
	uint8_t distanceCodesCount, codeLengthCodesCount;

	readBitsLittleEndian(in_pBitReadState, &literalCodesCount, 5);
	literalCodesCount += 257;

	if (literalCodesCount > 286)
		return ReadResultInvalidData;

	readBitsLittleEndian(in_pBitReadState, &distanceCodesCount, 5);
	distanceCodesCount += 1;

	if (distanceCodesCount > 32) // perhaps 30?
		return ReadResultInvalidData;

	readBitsLittleEndian(in_pBitReadState, &codeLengthCodesCount, 4);
	codeLengthCodesCount += 4;
	
	


	return ReadResultOK;
}

ReadResult (*pfTypeFunctions[4])(BitReadState *in_pBitReadState, 
	void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface) = {
	readDataNoCompression, 
	notImplemented, 
	readDataCompressedWithDynamicHuffmanCodes,
	invalidData
};

ReadResult parseRFC1951(void *in_out_pReadStreamState, ByteStreamReadInterface in_readInterface, 
	void *in_out_pWriteStreamState, ByteStreamWriteInterface in_writeInterface)
{
	BitReadState bitReadState;
	SetjmpStreamState setjmpStreamState;
	ByteStreamReadInterface setjmpStreamInterface = getSetjmpStreamReadReadInterface();
	jmp_buf jmpBuf;
	int result;

	uint8_t last, type;
	ReadResult readResult;

	// The = is correct here
	if (result = setjmpStreamInitAndSetjmp(&setjmpStreamState, &jmpBuf, ReadResultPrematureEndOfStream, 
		in_out_pReadStreamState, in_readInterface))
	{
		return (ReadResult) result;
	}

	initBitReadState(&bitReadState, &setjmpStreamState, setjmpStreamInterface);

	do
	{
		readBitsLittleEndian(&bitReadState, &last, 1);
		readBitsLittleEndian(&bitReadState, &type, 2);

		readResult = pfTypeFunctions[type](&bitReadState, in_out_pWriteStreamState, in_writeInterface);

		if (readResult != ReadResultOK)
			return readResult;
	} while (!last);

	return ReadResultOK;
}
