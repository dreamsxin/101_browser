/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "BasicDataStructures/Stream/ZReadStream.h"
#include <cstdlib>

bool ZReadStream::open(BasicReadStream* in_pStream)
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

void ZReadStream::close()
{
	if (isOpen() && mUsed)
		inflateEnd(&strm);

	mOpen = false;
	mUsed = false;
	mpStream = NULL;
}

bool ZReadStream::isOpen() const
{
	return mOpen;
}

size_t ZReadStream::read(void* in_buffer, size_t in_size, size_t in_count)
{
	if (!isOpen())
		return 0;

	strm.avail_out = in_size * in_count;
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
