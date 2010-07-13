#include "BasicDataStructures/Stream/FileReadStream.h"
#include "MiniStdlib/MTAx_fopen.h"

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
