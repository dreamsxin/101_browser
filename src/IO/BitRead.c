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

void initBitReadState(BitReadState *in_pBitReadState)
{
	in_pBitReadState->bitCountInBuffer = 0;
}

bool readBits(BitReadState *in_pBitReadState, void* in_pReaderState,  
	bool (*in_pReadByte)(void*, uint8_t*), void* in_pBuffer, size_t in_bitsCount)
{
	uint8_t *currentBufferElement = (uint8_t *) in_pBuffer;
	size_t currentBitIndex; // counter in the loop

	for (currentBitIndex = 0; currentBitIndex < in_bitsCount; ++currentBitIndex)
	{
		uint8_t currentBit;
		uint8_t position;

		if (in_pBitReadState->bitCountInBuffer == 0)
		{
			if (!in_pReadByte(in_pReaderState, &in_pBitReadState->buffer))
			{
				return false;
			}

			in_pBitReadState->bitCountInBuffer = 8;
		}

		assert(in_pBitReadState->bitCountInBuffer > 0);
		assert(in_pBitReadState->bitCountInBuffer <= 8);

		if (currentBitIndex % 8 == 0)
		{
			*currentBufferElement = 0;
		}

		position = (8-in_pBitReadState->bitCountInBuffer) % 8;
		currentBit = (in_pBitReadState->buffer >> position) & 1;
		*currentBufferElement |= (currentBit<<(currentBitIndex % 8));
		in_pBitReadState->bitCountInBuffer--;

		if ((currentBitIndex+1) % 8 == 0)
		{
			currentBufferElement++;
		}
	}

	return true;
}
