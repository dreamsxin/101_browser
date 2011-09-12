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

#include "Matroska/EBML.h"
#include "MiniStdlib/cassert.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstring.h" // memcmp

const uint8_t threeTimes0xFF[3] = { 0xFF, 0xFF, 0xFF };

// Helper functions

/*!
* Returns the remaing bytes to read.
* Will return UINT8_MAX if in_currentByte is invalid.
*/
uint8_t getRemainingBytes(uint8_t in_currentByte)
{
	if (in_currentByte & 0x80)
		return 0;
	else if (in_currentByte & 0x40)
		return 1;
	else if (in_currentByte & 0x20)
		return 2;
	else if (in_currentByte & 0x10)
		return 3;
	else if (in_currentByte & 0x08)
		return 4;
	else if (in_currentByte & 0x04)
		return 5;
	else if (in_currentByte & 0x02)
		return 6;
	else if (in_currentByte & 0x01)
		return 7;
	else
		return UINT8_MAX;
}

bool isReservedPossible(uint8_t in_firstByte, uint8_t in_remainingBytes)
{
	uint8_t unusedBits;
	
	assert(in_remainingBytes < 8);

	unusedBits = (0xFF << (8-in_remainingBytes));

	return (in_firstByte | unusedBits) == UINT8_MAX;
}

ReadResult readEbmlElementID(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, uint8_t out_elementID[4])
{
	bool isReservedIdPossible = false;
	uint8_t remainingBytes;

	if ((*in_readInterface.mpfRead)(in_out_pReadStreamState, 
		out_elementID+0, 1) != 1)
		return ReadResultPrematureEndOfStream;

	remainingBytes = getRemainingBytes(out_elementID[0]);

	if (remainingBytes > 3 || UINT8_MAX == remainingBytes)
		return ReadResultInvalidData;

	isReservedIdPossible = isReservedPossible(out_elementID[0], remainingBytes);

	if (remainingBytes > 0)
	{
		if ((*in_readInterface.mpfRead)(in_out_pReadStreamState, 
			out_elementID+1, remainingBytes) != remainingBytes)
			return ReadResultPrematureEndOfStream;
	}

	assert(remainingBytes <= 3);

	if (isReservedIdPossible && !memcmp(out_elementID+1, threeTimes0xFF, remainingBytes))
		return ReadResultInvalidData;

	return ReadResultOK;
}

ReadResult readEBmlElementSize(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, uint64_t *out_size)
{
	uint8_t currentByte, remainingBytes;
	bool isReservedSize; // gets initialized below

	if ((*in_readInterface.mpfRead)(in_out_pReadStreamState, 
		&currentByte, 1) != 1)
		return ReadResultPrematureEndOfStream;

	remainingBytes = getRemainingBytes(currentByte);

	if (UINT8_MAX == remainingBytes)
		return ReadResultInvalidData;

	isReservedSize = isReservedPossible(currentByte, remainingBytes);

	/*
	* Q: Why remainingBytes+1 and not remainingBytes?
	* A: Because we also have to mask away the leading 1.
	*/
	*out_size = currentByte & (0xFF >> (remainingBytes+1));

	while (remainingBytes)
	{
		if ((*in_readInterface.mpfRead)(in_out_pReadStreamState, 
			&currentByte, 1) != 1)
			return ReadResultPrematureEndOfStream;

		// Shift currentByte in from below
		*out_size = ((*out_size) << 8) | currentByte;

		if (isReservedSize && currentByte != 0xFF)
			isReservedSize = false;

		remainingBytes--;
	}

	if (isReservedSize)
		*out_size = UINT64_MAX;

	return ReadResultOK;
}
