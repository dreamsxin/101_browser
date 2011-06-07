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

#ifndef _MTAx_HTML_2_4_h
#define _MTAx_HTML_2_4_h

#include "MiniStdlib/declspec.h"
#include "IO/ByteStream.h"
#include "Util/ReadResult.h"

/*
 * 2.4 UTF-8
 * Living Standard — Last Updated 14 February 2011
 */

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT ReadResult convertUTF8toCodepoints(
	ByteStreamInterface in_readInterface, 
	void *in_pReadState,
	ByteStreamInterface in_writeInterface,
	void *in_pWriteState);

#ifdef __cplusplus
}
#endif

#endif
