#ifndef _BasicStream_h
#define _BasicStream_h

/*!
 * Implements a stream with the ability to be opened and closed
 */
template <typename OpenType> class BasicOpenStream
{
public:
	/*!
	 * Return value:
	 * true:  success
	 * false: failure (e. g. file not found or permission denied)
	 */
	virtual bool open(OpenType in_openType) = 0;
	
	/*!
	 * Closes the stream if it is open. If it is not open it does nothing
	 *
	 * It should also be called in the destructor (on the reason it is a pure
	 * virtual function here we can't use it in BasicStream<OpenType>'s 
	 * destructor)
	 */
	virtual void close() = 0;

	/*!
	 * At the beginning this must return false
	 * After open(...) is called it must return true
	 * After close() is called it must return false
	 */
	virtual bool isOpen() const = 0;
};

/*!
 * Implements a stream which can be read
 */
class BasicReadStream
{
public:
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

/*!
 * A stream which can also run "backward"
 */
class RevertibleReadStream : public BasicReadStream
{
public:
	virtual void revert(long in_offset) = 0;
};

#endif
