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

static ReadResult notImplemented(BitReadState *in_pBitReadState, ByteStreamReadInterface in_readStream) { return ReadResultNotImplemented; }
static ReadResult invalidData(BitReadState *in_pBitReadState, ByteStreamReadInterface in_readStream) { return ReadResultInvalidData; }

ReadResult readDataCompressedWithDynamicHuffmanCodes(BitReadState *in_pBitReadState, ByteStreamReadInterface in_readStream)
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

ReadResult (*pfTypeFunctions[4])(BitReadState *in_pBitReadState, ByteStreamReadInterface in_readStream) = {
	notImplemented, 
	notImplemented, 
	readDataCompressedWithDynamicHuffmanCodes,
	invalidData
};

ReadResult parseRFC1951(void *in_pStreamState, ByteStreamReadInterface in_readStream)
{
	BitReadState bitReadState;
	uint8_t last, type;
	ReadResult readResult;

	initBitReadState(&bitReadState, in_pStreamState, in_readStream);

	do
	{
		if (readBitsLittleEndian(&bitReadState, &last, 1) != 1)
			return ReadResultPrematureEndOfStream;
		if (readBitsLittleEndian(&bitReadState, &type, 2) != 2)
			return ReadResultPrematureEndOfStream;

		readResult = pfTypeFunctions[type](&bitReadState, in_readStream);

		if (readResult != ReadResultOK)
			return readResult;
	} while (!last);

	return ReadResultOK;
}
