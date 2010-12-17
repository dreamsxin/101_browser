#ifndef _MTAx_cstdio_h
#define _MTAx_cstdio_h

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

FILE* MTAx_fopen(const char *in_cFilename, const char *in_cMode);

/*
 * Closes the file and sets the value of *in_ppFile to NULL.
 * The reason why we always do so and not only in case of 
 * success is that fclose only returns an error if the stream
 * was already invalid. So also in this case it is better
 * to set it to NULL.
 */
int MTAx_fclose(FILE** in_ppFile);

#ifdef __cplusplus
}
#endif

#endif
