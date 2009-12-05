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
	 * the number of read structures of size in_size
	 */
	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count) = 0;
};

#endif
