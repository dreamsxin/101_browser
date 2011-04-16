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

ReadResult notImplemented(ByteStreamReadInterface in_readStream, void *in_pStreamState) { return ReadResultNotImplemented; }
ReadResult invalidData(ByteStreamReadInterface in_readStream, void *in_pStreamState) { return ReadResultInvalidData; }

ReadResult readDataCompressedWithDynamicHuffmanCodes(ByteStreamReadInterface in_readStream, void *in_pStreamState)
{
	return ReadResultOK;
}

ReadResult (*pfTypeFunctions[4])(ByteStreamReadInterface in_readStream, void *in_pStreamState) = {
	notImplemented, 
	notImplemented, 
	readDataCompressedWithDynamicHuffmanCodes,
	invalidData
};

ReadResult parseRFC1951(ByteStreamReadInterface in_readStream, void *in_pStreamState)
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

		readResult = pfTypeFunctions[type](in_readStream, in_pStreamState);

		if (readResult != ReadResultOK)
			return readResult;
	} while (!last);

	return ReadResultOK;
}
