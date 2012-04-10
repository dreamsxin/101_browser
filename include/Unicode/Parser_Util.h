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

/*
* Return value:
* 0 - success
* 1 (!= 0) - failure
*
* Q: Why the fiddling with out_pFailureEntryPointAdress and in_failureEntryPointValue
*    instead of simply passing some value?
* A: Because you can't rely that different enums have the same size.
*/
int emitCodepoint(void *in_out_pWriteState, ByteStreamWriteInterface_v4 in_writeInterface, 
	UnicodeCodePoint in_codePoint, void *out_pFailureEntryPointAdress, 
	size_t in_failureEntryPointSize, unsigned int in_failureEntryPointValue);

#ifdef __cplusplus
}
#endif

#endif
