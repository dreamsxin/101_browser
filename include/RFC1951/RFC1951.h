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

#ifndef _RFC1951_RFC1951_h
#define _RFC1951_RFC1951_h

#include "MiniStdlib/declspec.h"
#include "IO/ByteStreamInterface.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORT void parseRFC1951(ByteStreamReadInterface in_readStream, void *in_pStreamState);

#ifdef __cplusplus
}
#endif

#endif