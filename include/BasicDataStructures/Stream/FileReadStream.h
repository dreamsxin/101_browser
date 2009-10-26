#ifndef _FileReadStream_h
#define _FileReadStream_h

#include "BasicDataStructures/Stream/BasicStream.h"
#include <cstdio>

class FileReadStream : public BasicReadStream<const char*>
{
protected:
	FILE* mFile;

public:
	inline FileReadStream() : mFile(NULL) { }

	virtual bool open(char* in_openType);
	virtual void close();
	virtual bool isOpen();
	virtual size_t FileReadStream::read(void* in_buffer, size_t in_size, size_t in_count);
};

#endif
