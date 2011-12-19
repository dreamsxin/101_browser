/*
* Copyright 2011 Wolfgang Keller
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

#ifndef _Matroska_EBML_h
#define _Matroska_EBML_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/declspec.h"
#include "IO/ByteStream.h"
#include "Util/ReadResult.h"

typedef struct
{
	uint8_t ID[4];
	uint64_t size;
} EBML_ELEMENT;

#ifdef __cplusplus
extern "C" {
#endif

/*
* You probably better shoudn't use these functions directly.
* The only reason why they are marked as DLLEXPORT is for testing them in 
* TestSuite.
*
* If not ReadResultOK is returned, you may not make any assumptions about
* the content of the passed buffers after returning.
*/
DLLEXPORT ReadResult readEbmlElementID(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, uint8_t out_ID[4]);
DLLEXPORT ReadResult readEBml_elementSize(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, uint64_t *out_pSize);
DLLEXPORT ReadResult readEBml_vint(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, uint64_t *out_pVint);
DLLEXPORT ReadResult readEBml_svint(void *in_out_pReadStreamState, 
	ByteStreamInterface in_readInterface, int64_t *out_pVsint);

#ifdef __cplusplus
}
#endif

#endif
