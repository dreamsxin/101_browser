#ifndef _ArrayBlock_h
#define _ArrayBlock_h

#include "BasicDataStructures/Memory/SafeMemoryManagement.h"
#include <algorithm>
#include <cassert>

template <typename Type> struct ArrayBlock
{
protected:
	size_t mCount;
	Type* mData;

public:
	inline ArrayBlock<Type>() : mCount(0), mData(NULL) { }

	inline size_t count() const
	{
		return mCount;
	}

	inline Type* data() const
	{
		return mData;
	}

	inline void allocate(size_t in_count)
	{
		free();
		mCount=in_count;
		mData = safeMallocExitOnFailure<Type>(mCount);
	}

	inline void free()
	{
		mCount = 0;
		safe_free(&mData);
	}
};

#endif
