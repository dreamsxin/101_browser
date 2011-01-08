/*
 * Copyright 2008-2011 Wolfgang Keller
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
