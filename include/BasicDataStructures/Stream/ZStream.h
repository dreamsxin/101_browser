#include "BasicDataStructures/Stream/BasicStream.h"
#include "zlib.h"

class ZStream : public BasicReadStream, public BasicOpenStream<BasicReadStream*>
{
private:
	static const size_t bufferSize=16384;

	BasicReadStream* mpStream;
	bool mOpen;
	bool mUsed;
	z_stream strm;
	unsigned char mBuffer[bufferSize];

public:
	inline ZStream() : mOpen(false), mUsed(false) {

	}

	inline virtual ~ZStream()
	{
		close();
	}

	virtual bool open(BasicReadStream* in_pStream);
	virtual void close();

	/*!
	 * At the beginning this must return false
	 * After open(...) is called it must return true
	 * After close() is called it must return false
	 */
	virtual bool isOpen() const;

	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count);
};
