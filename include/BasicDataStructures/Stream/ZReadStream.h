#ifndef _ZReadStream_h
#define _ZReadStream_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicReadStream.h"
#include "zlib.h"

class ZReadStream : public BasicReadStream, public BasicOpenStream<BasicReadStream*>
{
private:
	static const size_t bufferSize=0x4000;

	BasicReadStream* mpStream;
	bool mOpen;
	bool mUsed;
	z_stream strm;
	unsigned char mBuffer[bufferSize];

public:
	inline ZReadStream() : mOpen(false), mUsed(false) {

	}

	inline virtual ~ZReadStream()
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

#endif
