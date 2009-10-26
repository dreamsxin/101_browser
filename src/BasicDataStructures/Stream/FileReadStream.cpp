#include "BasicDataStructures/Stream/FileReadStream.h"

bool FileReadStream::open(char* in_filename)
{
	closeIfOpen();
	mFile = fopen(in_filename, "r");
	return isOpen();
}

bool FileReadStream::isOpen()
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
	return fread(in_buffer, in_size, in_count, mFile);
}
