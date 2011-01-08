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

#ifndef _FileReadStream_h
#define _FileReadStream_h

#include "BasicDataStructures/Stream/BasicOpenStream.h"
#include "BasicDataStructures/Stream/RevertibleReadStream.h"
#include <cstdio>

class FileReadStream : public BasicOpenStream<const char*>, public RevertibleReadStream
{
private:
	FILE* mFile;

	inline FileReadStream& operator=(FileReadStream& in_frs) { return *this; }
	inline FileReadStream(const FileReadStream& in_s) { }

public:
	inline FileReadStream() : mFile(NULL) { }

	inline virtual ~FileReadStream()
	{
		close();
	}

	virtual bool open(const char* in_openType);
	virtual void close();
	virtual bool isOpen() const;
	virtual size_t read(void* in_buffer, size_t in_size, size_t in_count);
	virtual void revert(long in_offset);
};

#endif
