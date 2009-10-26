#ifndef _BasicStream_h
#define _BasicStream_h

template <typename OpenType> class BasicStream
{
public:
	inline virtual ~BasicStream()
	{
		closeIfOpen();
	}

	/*!
	 * Return value:
	 * true:  success
	 * false: failure (e. g. file not found or permission denied)
	 */
	virtual bool open(OpenType in_openType) = 0;
	virtual void close() = 0;

	/*!
	 * At the beginning this must return false
	 * After open(...) is called it must return true
	 * After close() is called it must return false
	 */
	virtual bool isOpen() const = 0;

	/*!
	 * A little helper function which usually should be called at the beginning 
	 * of each open(...) implementation
	 */
	inline void closeIfOpen()
	{
		if (isOpen())
			close();
	}
};

template <typename OpenType> class BasicReadStream : public BasicStream<OpenType>
{
public:
	/*!
	 * Return value:
	 * the number of read bytes
	 */
	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count);
};

#endif
