#ifndef _FileReadStream_h
#define _FileReadStream_h

#include "BasicDataStructures/Stream/BasicStream.h"
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
