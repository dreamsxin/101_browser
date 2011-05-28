/*
 * Copyright 2011 Wolfgang Keller
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

#include "IO/MemoryByteStream.h"
#include "MiniStdlib/minmax.h"
#include <assert.h>
#include <string.h> // for memcpy

void memoryByteStreamInit(MemoryByteStream *in_pMemoryByteStream,
	const void *in_pBuffer, size_t in_bufferSize)
{
	memset(&in_pMemoryByteStream->mByteStreamInterface, 0, 
		sizeof(in_pMemoryByteStream->mByteStreamInterface));
	in_pMemoryByteStream->mByteStreamInterface.mpfRead = memoryByteReadStreamRead;

	in_pMemoryByteStream->buffer = (const uint8_t*) in_pBuffer;
	in_pMemoryByteStream->bufferSize = in_bufferSize;
	in_pMemoryByteStream->bufferPos = 0;
}

size_t memoryByteReadStreamRead(void *in_out_pMemoryByteStream, 
	void *out_pBuffer, size_t in_count)
{
	MemoryByteStream *pMemoryByteStream = 
		(MemoryByteStream *) in_out_pMemoryByteStream;

	in_count = MIN(in_count, pMemoryByteStream->bufferSize - pMemoryByteStream->bufferPos);
	memcpy(out_pBuffer, pMemoryByteStream->buffer+pMemoryByteStream->bufferPos, in_count);
	pMemoryByteStream->bufferPos += in_count;

	assert(pMemoryByteStream->bufferPos <= pMemoryByteStream->bufferSize);

	return in_count;
}
