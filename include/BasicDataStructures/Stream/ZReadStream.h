#ifndef _ZReadStream_h
#define _ZReadStream_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/BasicReadStream.h"
#include "zlib.h"
#include <cstdlib>

class ZReadStream : public BasicOpenStream<BasicReadStream*>, public BasicReadStream
{
private:
	static const size_t bufferSize=0x4000;

	BasicReadStream* mpStream;
	bool mOpen;
	/**
	 * has data been read and uncompressed? If yes this flag is set to true
	 *
	 * This is important, since if this is the case we have to call inflateEnd
	 * when closing the stream
	 */
	bool mUsed;
	z_stream strm;
	unsigned char mBuffer[bufferSize];

public:
	inline ZReadStream() : mOpen(false), mUsed(false), mpStream(NULL) {

	}

	inline virtual ~ZReadStream()
	{
		close();
	}

	virtual bool open(BasicReadStream* in_pStream);
	virtual void close();
	virtual bool isOpen() const;

	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count);
};

#endif
