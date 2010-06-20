#ifndef _FileArray_h
#define _FileArray_h

#include <cstdio>

namespace MTAx
{
	namespace Array
	{
		struct FileArray
		{
			FILE* file;
			fpos_t pos;
		};


	}
}

#endif
