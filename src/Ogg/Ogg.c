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

#include "Ogg/Ogg.h"
#include "Ogg/Skeleton.h"
#include "MiniStdlib/cstring.h"
#include "MiniStdlib/MTAx_cstdio.h"

const char OggS[4] = { 'O', 'g', 'g', 'S' };

ReadResult readAndCheckOggPageHeader(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, OggPageHeader *out_pOggPageHeader)
{
	if (in_readInterface.mpfRead(in_out_pReadStreamState, out_pOggPageHeader, 
		sizeof(OggPageHeader)) != sizeof(OggPageHeader))
		return ReadResultPrematureEndOfStream;

	if (memcmp(out_pOggPageHeader->capture_pattern, OggS, 4))
		return ReadResultInvalidData;

	if (out_pOggPageHeader->stream_structure_version != 0)
		return ReadResultNotImplemented;

	if (out_pOggPageHeader->header_type_flag.unused0 || 
		out_pOggPageHeader->header_type_flag.unused1)
		return ReadResultInvalidData;

	return ReadResultOK;
}

ReadResult readOgg(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface)
{
	OggPageHeader oggPageHeader;
	ReadResult readResult;
	int count = 0;

	if ((readResult = readAndCheckOggPageHeader(in_out_pReadStreamState, 
		in_readInterface, &oggPageHeader)) != ReadResultOK)
		return readResult;

	if (!oggPageHeader.header_type_flag.bos)
		return ReadResultInvalidData;

	if (oggPageHeader.number_page_segments == 0x01)
	{
		uint8_t lacingValue;

		if (in_readInterface.mpfRead(in_out_pReadStreamState, &lacingValue, 1) != 1)
			return ReadResultPrematureEndOfStream;

		if (lacingValue == 0x40)
		{
			FisheadIdentHeader fisheadIdentHeader;

			if (in_readInterface.mpfRead(in_out_pReadStreamState, &fisheadIdentHeader, 
				sizeof(FisheadIdentHeader)) != sizeof(FisheadIdentHeader))
				return ReadResultPrematureEndOfStream;

			if (memcmp(fisheadIdentHeader.Identifier, "fishead", 8))
				return ReadResultNotImplemented;

			if (fisheadIdentHeader.Version_major != 0x3 ||
				fisheadIdentHeader.Version_minor != 0x0)
				return ReadResultNotImplemented;
		}
		else
			return ReadResultNotImplemented;
	}
	else
		return ReadResultNotImplemented;

	while (!oggPageHeader.header_type_flag.eos)
	{
		count++;

		if ((readResult = readAndCheckOggPageHeader(in_out_pReadStreamState, 
			in_readInterface, &oggPageHeader)) != ReadResultOK)
			return readResult;

		if (oggPageHeader.number_page_segments == 0x01)
		{
			uint8_t byte;
			uint8_t idx;
			uint8_t lacingValue;

			if (in_readInterface.mpfRead(in_out_pReadStreamState, &lacingValue, 1) != 1)
				return ReadResultPrematureEndOfStream;

			for (idx = 0; idx < lacingValue; idx++)
			{
				if (in_readInterface.mpfRead(in_out_pReadStreamState, &byte, 1) != 1)
					return ReadResultPrematureEndOfStream;
			}
		}
		else
		{
			printf("Count: %i\n", count);
			return ReadResultNotImplemented;
		}
	}

	return ReadResultOK;
}
