#include "BasicDataStructures/Stream/ZStream.h"
#include <cstdlib>

bool ZStream::open(BasicReadStream* in_pStream)
{
	close();

	mpStream = in_pStream;
	
	int ret;

	strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);

	mOpen = (ret == Z_OK);

    return isOpen();
}

void ZStream::close()
{
	if (isOpen() && mUsed)
		inflateEnd(&strm);

	mOpen = false;
	mUsed = false;
}

bool ZStream::isOpen() const
{
	return mOpen;
}

size_t ZStream::read(void* in_buffer, size_t in_size, size_t in_count)
{
	if (!isOpen())
		return 0;
	
	strm.avail_out = in_size* in_count;
	strm.next_out = (unsigned char*) in_buffer;

	do
	{
		if (strm.avail_in == 0)
		{
			strm.avail_in = mpStream->read(mBuffer, 1, bufferSize);

			if (strm.avail_in == 0)
			{
				close();
				break;
			}
			strm.next_in = mBuffer;
		}
		
		int ret = inflate(&strm, Z_SYNC_FLUSH);

		switch (ret)
		{
			case Z_NEED_DICT:
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
				close();
		}

		mUsed = true;
	} while (strm.avail_out != 0);

	return (in_size* in_count-strm.avail_out)/in_size;
}
