#ifndef _BasicReadStream_h
#define _BasicReadStream_h

/*!
 * Implements a stream which can be read
 */
class BasicReadStream
{
public:
	virtual ~BasicReadStream() { }

	/*!
	 * Return value:
	 * the number of read bytes
	 * 
	 * This function may call close() on the stream if there is no more data
	 * to fetch (but it MUST NOT do this if the implementing class is a 
	 * subclass of RevertibleReadStream)
	 */
	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count) = 0;
};

#endif
