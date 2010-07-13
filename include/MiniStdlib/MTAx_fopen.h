#ifndef _MTAx_fopen_h
#define _MTAx_fopen_h

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif
#ifdef __cplusplus
extern "C" {
#endif

FILE* MTAx_fopen(const char *in_cFilename, const char *in_cMode);

#ifdef __cplusplus
}
#endif

#endif
