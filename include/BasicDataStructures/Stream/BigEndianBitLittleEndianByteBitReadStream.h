#ifndef _BigEndianBitLittleEndianByteBitReadStream_h
#define _BigEndianBitLittleEndianByteBitReadStream_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicReadStream.h"
#include <cstdlib>

class BigEndianBitLittleEndianByteBitReadStream 
	:  public BasicOpenStream<BasicReadStream*>, public BasicReadStream
{
protected:
	/*!
	 * the stream to pull the data from
	 */
	BasicReadStream* mpStream;
	/*!
	 * the number of remaining bits until a new byte has to be read
	 */
	unsigned char mRemainingBitsCount;
	/*!
	 * the last read byte. For further bit partitioning
	 */
	unsigned char mCurrentByte;

	/*!
	 * Reads a bit. Return value:
	 * 1 - a bit was read
	 * 0 - no bit was read
	 *
	 * in_buffer: buffer to a single character
	 */
	unsigned char readBit(unsigned char* in_buffer);

public:
	inline BigEndianBitLittleEndianByteBitReadStream() 
		: mpStream(NULL), mRemainingBitsCount(0) {

	}

	inline virtual ~BigEndianBitLittleEndianByteBitReadStream()
	{
		close();
	}

	virtual bool open(BasicReadStream* in_pStream);
	virtual void close();
	virtual bool isOpen() const;

	/*!
	 * Important: in_size is in bits, not in bytes. Also it is returned how many
	 * bits were read.
	 */
	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count);
};

#endif
