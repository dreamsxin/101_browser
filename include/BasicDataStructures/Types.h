#ifndef _Types_h
#define _Types_h

#include "BasicDataStructures/PlatformIndependentHeaders/cstdint.h"

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
