#include "BasicDataStructures/Stream/FileReadStream.h"

bool FileReadStream::open(const char* in_filename)
{
	closeIfOpen();
	mFile = fopen(in_filename, "r");
	return isOpen();
}

bool FileReadStream::isOpen() const
{
	return mFile != NULL;
}

void FileReadStream::close()
{
	if (mFile != NULL)
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
