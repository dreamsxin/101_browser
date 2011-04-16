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

#ifndef _MTAx_IO_FileByteStream_h
#define _MTAx_IO_FileByteStream_h

#include "MiniStdlib/MTAx_cstdio.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/declspec.h"
#include "IO/ByteStreamInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	FILE *mFile;
} FileByteStreamState;

DLLEXPORT size_t fileByteReadStreamRead(void *in_out_pFileByteStreamState, 
	void *out_pBuffer, size_t in_count);

DLLEXPORT bool fileByteReadStreamState_create(const char *in_filename,
	FileByteStreamState *in_pFileByteStreamState);

DLLEXPORT void fileByteReadStreamState_destroy(FileByteStreamState *in_pFileByteStreamState);

const ByteStreamReadInterface cFileByteStreamInterface = { &fileByteReadStreamRead, NULL };

#ifdef __cplusplus
}
#endif

#endif
