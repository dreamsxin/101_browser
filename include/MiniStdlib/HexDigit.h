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

#ifndef _MTAx_MiniStdlib_HexDigit_h
#define _MTAx_MiniStdlib_HexDigit_h

#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/HexDigit.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * convertHexDigitToNumber converts the hex token token (i. e. token obeys 
 * (token >= '0' && token <= '9') || (token >= 'A' && token <= 'F')
 * ) into the corresponding number.
 * 
 * If token does not obey this condition 0xFF ((uint8_t) -1) is returned.
 */
uint8_t convertHexDigitToNumber(char token);

#ifdef __cplusplus
}
#endif

#endif
