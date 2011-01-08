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

#include "BasicDataStructures/Stream/FileReadStream.h"
#include "MiniStdlib/MTAx_cstdio.h"

bool FileReadStream::open(const char* in_filename)
{
	close();

	mFile = MTAx_fopen(in_filename, "rb");

	return isOpen();
}

bool FileReadStream::isOpen() const
{
	return mFile != NULL;
}

void FileReadStream::close()
{
	if (isOpen())
		fclose(mFile);

	mFile = NULL;
}

size_t FileReadStream::read(void* in_buffer, size_t in_size, size_t in_count)
{
	if (isOpen())
		return fread(in_buffer, in_size, in_count, mFile);
	else
		return 0;
}

void FileReadStream::revert(long in_offset)
{
	fseek(mFile, -in_offset, SEEK_CUR);
}
