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

#include "IO/FileByteStream.h"
#include "IO/ByteStreamInterface.h"


size_t fileByteReadStreamRead(void *in_out_pFileByteStreamState, 
	void *out_pBuffer, size_t in_count)
{
	FileByteStreamState *pFileByteStreamState = 
		(FileByteStreamState*) in_out_pFileByteStreamState;

	return fread(out_pBuffer, 1, in_count, pFileByteStreamState->mFile);
}

bool fileByteReadStreamState_create(const char *in_filename,
	FileByteStreamState *in_pFileByteStreamState)
{
	in_pFileByteStreamState->mFile = MTAx_fopen(in_filename, "rb");

	return in_pFileByteStreamState->mFile != NULL;
}

void fileByteReadStreamState_destroy(FileByteStreamState *in_pFileByteStreamState)
{
	fclose(in_pFileByteStreamState->mFile);
	in_pFileByteStreamState->mFile = NULL;
}