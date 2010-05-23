#ifndef _SafeMemoryManagement_h
#define _SafeMemoryManagement_h

#include <cstdlib>
#include "BasicDataStructures/Error/ErrorHandling.h"

template <typename T> T* safeMallocBytesExitOnFailure(size_t in_bytesCount)
{
	T* out_pointer = static_cast<T*>(malloc(in_bytesCount));

	if (!out_pointer)
	{
		showErrorMessageAndExit(L"malloc()");
	}

	return out_pointer;
}

template <typename T> T* safeMallocExitOnFailure(size_t in_count)
{
	return safeMallocBytesExitOnFailure<T>(in_count*sizeof(T));
}

template <typename T> void safe_free(T** in_ppT)
{
	free(*in_ppT);
	*in_ppT = NULL;
}

template <typename T> void safe_delete(T** in_ppT)
{
	delete *in_ppT;
	*in_ppT = NULL;
}

template <typename T> void safe_delete_array(T** in_ppT)
{
	delete[] *in_ppT;
	*in_ppT = NULL;
}

#endif
