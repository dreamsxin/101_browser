#ifndef _BasicWriteStream_h
#define _BasicWriteStream_h

class BasicWriteStream
{
	/*!
	 * Return value:
	 * the number of written bytes
	 */
	virtual size_t write(const void* in_buffer, size_t in_size, size_t in_count) = 0;
};

#endif
