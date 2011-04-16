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

void initBitReadState(BitReadState *in_pBitReadState, void *in_pByteStreamState, 
	ByteStreamReadInterface in_readInterface)
{
	in_pBitReadState->bitCountInBuffer = 0;
	/*
	* There is no need to initialize in_pBitReadState->buffer
	* since we set in_pBitReadState->bitCountInBuffer = 0
	* so that the buffer contains no data.
	*/
	in_pBitReadState->pByteStreamState = in_pByteStreamState;
	in_pBitReadState->readInterface = in_readInterface;
}

size_t readBitsLittleEndian(BitReadState *in_pBitReadState, void* out_pBuffer, size_t in_bitsCount)
{
	uint8_t *currentBufferElement = (uint8_t *) out_pBuffer;
	size_t currentBitIndex; // counter in the loop

	for (currentBitIndex = 0; currentBitIndex < in_bitsCount; ++currentBitIndex)
	{
		uint8_t currentBit;
		uint8_t position;

		if (in_pBitReadState->bitCountInBuffer == 0)
		{
			if ((*in_pBitReadState->readInterface.pRead)(in_pBitReadState->pByteStreamState, &in_pBitReadState->buffer, 1) != 1)
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

		position = (8-in_pBitReadState->bitCountInBuffer);
		currentBit = (in_pBitReadState->buffer >> position) & 1;
		*currentBufferElement |= (currentBit<<(currentBitIndex % 8));
		in_pBitReadState->bitCountInBuffer--;

		if ((currentBitIndex+1) % 8 == 0)
		{
			currentBufferElement++;
		}
	}

	return in_bitsCount;
}
