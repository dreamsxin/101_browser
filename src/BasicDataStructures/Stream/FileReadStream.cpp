#include "BasicDataStructures/Stream/FileReadStream.h"
#include "BasicDataStructures/PlatformIndependentFunctions/fopen_s.h"

bool FileReadStream::open(const char* in_filename)
{
	close();
	if (pi_fopen_s(&mFile, in_filename, "rb") != 0)
		return false;
	else
		return true;
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
