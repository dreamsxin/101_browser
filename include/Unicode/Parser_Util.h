/*
* Copyright 2012 Wolfgang Keller
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

#ifndef _Unicode_Parser_Util_h
#define _Unicode_Parser_Util_h

#include "Unicode/Parser.h"

#ifdef __cplusplus
extern "C" {
#endif

void parserStateInit(ParserState *out_pParserState, 
	void *in_pReadState, 
	ByteStreamReadInterface_v3 in_readInterface);

ByteStreamReadInterface_v3 getParser_ReadInterface(
	size_t (*mpfReadFunction)(void *in_out_pByteStreamState, 
		void *out_pBuffer, size_t in_count));

void writeCodePoint(UnicodeCodePoint **out_ppBuffer, 
	size_t *in_out_pWriteCount, UnicodeCodePoint in_codePoint);

#ifdef __cplusplus
}
#endif

#endif
