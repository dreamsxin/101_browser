#ifndef _Types_h
#define _Types_h

/*
 * Visual Studio 2010 is the first version to 
 * contains cstdint. In the versions before we
 * have to use boost
 */
#if defined(_MSC_VER) && (_MSC_VER<=1500)
#include "boost/cstdint.hpp"
using namespace boost; // This is a hack - you should not use this in header files
#else
#include <cstdint>
#endif

#ifdef _WIN32
#include "windows.h"
#else
typedef unsigned int        UINT;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef unsigned short      USHORT;

#endif

struct Fixed
{
	 SHORT beforeComma;
	USHORT  afterComma;
};

#ifdef _WIN32
typedef __int64 LONGDATETIME;
#else
typedef long long LONGDATETIME;
#endif

#endif
