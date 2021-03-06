/*
* Copyright 2008-2012 Wolfgang Keller
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

#ifndef _MTAx_MiniStdlib_HexDigit_h
#define _MTAx_MiniStdlib_HexDigit_h

#include "Unicode/Unicode.h"
#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

bool convertHexDigitToNumber(UnicodeCodePoint token, uint8_t *out_pNumber, 
	bool in_acceptUppercase, bool in_acceptLowercase);

void convertNumberToHexDigits(uint8_t in_number, bool in_lowercase, char out_characters[2]);

#ifdef __cplusplus
}
#endif

#endif
