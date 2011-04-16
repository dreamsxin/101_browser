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

#ifndef _MTAx_IO_ByteStreamInterface_h
#define _MTAx_IO_ByteStreamInterface_h

#include "MiniStdlib/cstdint.h"
/* 
 * gcc requires this header file for a definition of size_t
 */
#include "MiniStdlib/cstddef.h"
#include "MiniStdlib/cstdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	size_t (*pRead)(void *in_out_pByteStreamState, void *out_pBuffer, size_t in_count);
	/*
	* An optional capability. If it is implemented (i. e. not NULL) the following properties
	* must hold
	* 1. *pIsTerminated is idempotent
	* 2. if pIsTerminated returns true alls calls to *pRead must return 0
	* 3. if pIsTerminated returns false *pRead must return a value > 0
	* 4. if pRead returns less than in_count pIsTerminated must return 0
	* 5. if at some point *pIsTerminated returns false it has to stay so in future
	*/
	bool (*pIsTerminated)(void *in_pByteStreamState);
} ByteStreamReadInterface;

typedef struct
{
	size_t (*pWrite)(void *in_out_pByteStreamState, const void *in_pBuffer, size_t in_count);
} ByteStreamWriteInterface;

#ifdef __cplusplus
}
#endif

#endif
