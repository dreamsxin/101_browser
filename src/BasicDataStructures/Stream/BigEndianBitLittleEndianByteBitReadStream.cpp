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

#include "BasicDataStructures/Stream/BigEndianBitLittleEndianByteBitReadStream.h"
#include <cstring>

unsigned char BigEndianBitLittleEndianByteBitReadStream::readBit(unsigned char* in_buffer)
{
	if (mRemainingBitsCount==0)
	{
		if (mpStream->read(&mCurrentByte, 1, 1)!=1)
		{
			return 0;
		}

		mRemainingBitsCount=8;
	}
	
	mRemainingBitsCount--;
	unsigned char mCurrentBit = (mCurrentByte & (1<<mRemainingBitsCount))!=0;
	*in_buffer<<=1;
	*in_buffer|=mCurrentBit;
	return 1;
}

bool BigEndianBitLittleEndianByteBitReadStream::open(BasicReadStream* in_pStream)
{
	mpStream = in_pStream;
	mRemainingBitsCount = 0;

	return true;
}

void BigEndianBitLittleEndianByteBitReadStream::close()
{
	mpStream = NULL;
}

bool BigEndianBitLittleEndianByteBitReadStream::isOpen() const
{
	return mpStream != NULL;
}

size_t BigEndianBitLittleEndianByteBitReadStream::read
(void* in_buffer, size_t in_size, size_t in_count)
{
	return readBits(in_buffer, 8*in_size, in_count);
}

size_t BigEndianBitLittleEndianByteBitReadStream::readBits
(void* in_buffer, size_t in_size, size_t in_count)
{
	unsigned char bitIndex = 0;

	// the number of complete bytes of the structure to read
	size_t structCompleteBytesCount = in_size / 8;
	unsigned char structRemainingBits = in_size % 8;

	// the number of bytes of the structure to fill
	size_t structBytesCount = structCompleteBytesCount + (structRemainingBits ? 1 : 0);

	unsigned char* currentPosition = (unsigned char*) in_buffer;

	size_t readStructsCount = 0;

	for (size_t currentStructIndex=0; currentStructIndex<in_count; currentStructIndex++)
	{
		for (unsigned char currentStructRemainingBit=0; 
			currentStructRemainingBit<structRemainingBits; currentStructRemainingBit++)
		{
			if (readBit(currentPosition)==0)
				goto end_of_read;
		}

		if (structRemainingBits>0)
			currentPosition++;

		for (size_t currentCompleteByte = 0; currentCompleteByte < structCompleteBytesCount; 
			currentCompleteByte++)
		{
			for (unsigned char currentCompleteByteBit = 0; currentCompleteByteBit < 8; 
				currentCompleteByteBit++)
			{
				if (readBit(currentPosition)==0)
					goto end_of_read;
			}

			currentPosition++;
		}

		readStructsCount++;
	}

end_of_read:
	return readStructsCount;
}

size_t BigEndianBitLittleEndianByteBitReadStream::readBits
(void* in_buffer, size_t in_size, size_t in_count, size_t in_blockSize)
{
	unsigned char* currentPosition = (unsigned char*) in_buffer;
	size_t readBlocksCount = 0;

	for (size_t currentBlockIndex = 0; currentBlockIndex < in_count; currentBlockIndex++)
	{
		memset(currentPosition, 0, in_blockSize);

		if (read(currentPosition, in_size, 1)!=1)
			break;

		currentPosition+=in_blockSize;
		readBlocksCount++;
	}

	return readBlocksCount;
}
