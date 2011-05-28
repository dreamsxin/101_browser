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

#include "IO/BitRead.h"
#include <assert.h>
#include <string.h>

void bitReadStateInit(BitReadState *out_pBitReadState, void *in_pSuperByteStream)
{
	assert(((ByteStreamInterface*) in_pSuperByteStream)->mpfRead != NULL);
	
	out_pBitReadState->bitCountInBuffer = 0;
	/*
	* There is no need to initialize in_pBitReadState->buffer
	* since we set in_pBitReadState->bitCountInBuffer = 0
	* so that the buffer contains no data.
	*/
	out_pBitReadState->mpSuperByteStream = in_pSuperByteStream;
}

size_t readBits(BitReadState *in_pBitReadState, void* out_pBuffer, size_t in_bitsCount, bool in_littleEndian)
{
	uint8_t *currentBufferElement = (uint8_t *) out_pBuffer;
	size_t currentBitIndex; // counter in the loop

	for (currentBitIndex = 0; currentBitIndex < in_bitsCount; ++currentBitIndex)
	{
		uint8_t currentBit;
		uint8_t position;

		if (in_pBitReadState->bitCountInBuffer == 0)
		{
			ByteStreamInterface *in_pByteStreamInterface = (ByteStreamInterface*) 
				in_pBitReadState->mpSuperByteStream;
			
			if ((*in_pByteStreamInterface->mpfRead)(in_pBitReadState->mpSuperByteStream, &in_pBitReadState->buffer, 1) != 1)
			{
				return currentBitIndex;
			}

			in_pBitReadState->bitCountInBuffer = 8;
		}

		assert(in_pBitReadState->bitCountInBuffer > 0);
		assert(in_pBitReadState->bitCountInBuffer <= 8);

		if (currentBitIndex % 8 == 0)
		{
			*currentBufferElement = 0;
		}

		if (in_littleEndian)
		{
			// position is the bit index of the bit that we want to read now
			position = (8-in_pBitReadState->bitCountInBuffer);
			// we copy the bit at position into currentBit
			currentBit = (in_pBitReadState->buffer >> position) & 1;
			// we shift it to the right position
			*currentBufferElement |= (currentBit<<(currentBitIndex % 8));
		}
		else
		{
			// position is the bit index of the bit that we want to read now
			position = in_pBitReadState->bitCountInBuffer;
			// we copy the bit at position into currentBit
			currentBit = (in_pBitReadState->buffer >> position) & 1;
			// Shift the new bit into the buffer from the low side
			*currentBufferElement <<= 1;
			*currentBufferElement |= currentBit;
		}

		in_pBitReadState->bitCountInBuffer--;

		if ((currentBitIndex+1) % 8 == 0)
		{
			currentBufferElement++;
		}
	}

	assert(currentBitIndex == in_bitsCount);

	return in_bitsCount;
}

size_t readBitsLittleEndian(BitReadState *in_pBitReadState, void* out_pBuffer, size_t in_bitsCount)
{
	return readBits(in_pBitReadState, out_pBuffer, in_bitsCount, true);
}

size_t readBitsBigEndian(BitReadState *in_pBitReadState, void* out_pBuffer, size_t in_bitsCount)
{
	return readBits(in_pBitReadState, out_pBuffer, in_bitsCount, false);
}

void bitReadStateFlush(BitReadState *in_out_pBitReadState)
{
	in_out_pBitReadState->bitCountInBuffer = 0;
}
