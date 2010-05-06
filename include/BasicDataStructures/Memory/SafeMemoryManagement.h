#ifndef _SafeMemoryManagement_h
#define _SafeMemoryManagement_h

#include <cstdlib>

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
