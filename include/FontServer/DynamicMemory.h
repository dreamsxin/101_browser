#ifndef _DynamicMemory_h
#define _DynamicMemory_h

#ifdef _WIN32
#include <windows.h>
#else
typedef void *LPVOID;
#endif
#include <algorithm>

class DynamicMemory
{
private:
	size_t mSize;
	LPVOID mBuf;

	inline DynamicMemory(const DynamicMemory& dynamicMemory) { }
	inline DynamicMemory operator=(const DynamicMemory& dynamicMemory) { }

public:
	inline DynamicMemory(size_t in_size) : mSize(in_size)
	{
		mBuf = malloc(size());
	}

	inline ~DynamicMemory()
	{
		free(buffer());
	}

	inline size_t size() const
	{
		return mSize;
	}

	inline LPVOID buffer() const
	{
		return mBuf;
	}

	/*!
	Exchanges the DynamicMemory structures
	*/
	inline DynamicMemory& operator^=(DynamicMemory& in_memory)
	{
		std::swap(mSize, in_memory.mSize);
		std::swap(mBuf, in_memory.mBuf);

		return *this;
	}
};

#endif

