#include "BasicDataStructures/Stream/BigEndianBitLittleEndianByteBitReadStream.h"

bool BigEndianBitLittleEndianByteBitReadStream::open(BasicReadStream* in_pStream)
{
	mpStream = in_pStream;
	mBitsRemaining = 0;

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

