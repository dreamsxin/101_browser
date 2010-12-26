#ifndef _fread_h
#define _fread_h

#include "MiniStdlib/MTAx_cstdio.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
__declspec(dllexport)
#endif
size_t fread_withState(void *in_dstBuffer, size_t in_elementSize, size_t in_count, FILE* in_file,
	void *in_pState, void (*in_pHandler)(void* in_pState, const void* in_pBuffer, size_t in_count));

#ifdef __cplusplus
}
#endif

#endif