#ifndef _MTAx_cstdlib_h
#define _MTAx_cstdlib_h

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _GCC
inline dword _byteswap_ulong(dword x)
{
	asm("bswap %0" : "=r" (x) : "0" (x));
	return x;
}

inline uint64 _byteswap_uint64(uint64 x)
{
	asm("bswap %0" : "=r" (x) : "0" (x));
	return x;
}

inline word _byteswap_ushort(word x)
{
	__asm__("xchgb %b0,%h0" : "=q" (x) :  "0" (x));
	return x;
}
#endif

#ifdef __cplusplus
}
#endif

#endif
