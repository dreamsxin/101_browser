#ifndef _OZO_fopen_s_h
#define _OZO_fopen_s_h

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

errno_t pi_fopen_s(FILE ** in_ppFile, const char *in_cFilename, const char *in_cMode);

#ifdef __cplusplus
}
#endif

#endif
