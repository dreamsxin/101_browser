#include "BasicDataStructures/Stream/BigEndianBitLittleEndianByteBitReadStream.h"

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
	unsigned char bitIndex = 0;

	// the number of complete bytes of the structure to read
	size_t structCompleteBytesCount = in_size / 8;
	unsigned char structRemainingBits = in_size % 8;

	// the number of bytes of the structure to fill
	size_t structBytesCount = structCompleteBytesCount + structRemainingBits ? 1 : 0;

	unsigned char* currentPosition = (unsigned char*) in_buffer;

	size_t readBitsCount = 0;

	for (size_t currentStructIndex=0; currentStructIndex<in_count; currentStructIndex++)
	{
		for (unsigned char currentStructRemainingBit=0; 
			currentStructRemainingBit<structRemainingBits; currentStructRemainingBit++)
		{
			if (readBit(currentPosition)==0)
				return readBitsCount;
			else
				readBitsCount++;
		}

		if (structRemainingBits>0)
			currentPosition++;

		for (size_t currentCompleteByte = 0; currentCompleteByte < structCompleteBytesCount; 
			currentCompleteByte)
		{
			for (unsigned char currentCompleteByteBit = 0; currentCompleteByteBit < 8; 
				currentCompleteByteBit++)
			{
				if (readBit(currentPosition)==0)
					return readBitsCount;
				else
					readBitsCount++;
			}

			currentPosition++;
		}
	}

	return readBitsCount;
}
