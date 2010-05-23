#ifndef _ArrayBlock_h
#define _ArrayBlock_h

#include "BasicDataStructures/Memory/SafeMemoryManagement.h"
#include <algorithm>

template <typename Type> struct ArrayBlock
{
protected:
	size_t mCount;
	Type* mData;

public:
	inline ArrayBlock<Type>() : mCount(0)
	{
		mData = safeMallocExitOnFailure<Type>(mCount);
	}

	inline ArrayBlock<Type>(size_t in_count) : mCount(in_count)
	{
		mData = safeMallocExitOnFailure<Type>(mCount);
	}

	inline size_t count() const
	{
		return mCount;
	}

	inline Type* data() const
	{
		return mData;
	}
};

#endif
