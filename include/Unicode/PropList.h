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

#ifndef _PropList_h
#define _PropList_h

/* For FILE */
#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#include "MiniStdlib/cstdbool.h"
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/HexDigit.h"
#include "MiniStdlib/declspec.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * out_ppIntervals has to be a pointer to a pointer to Interval<UnicodeCodePoint>;
 * the intervals will be allocated
 */
DLLEXPORT bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals, size_t* out_intervalsCount);

/*!
 * in_ppIntervals has to be a pointer to a pointer to Interval<UnicodeCodePoint>;
 * the intervals will be freed
 */
DLLEXPORT void freeIntervalsFromPropList(void* in_ppIntervals);

/*!
 * convertHexDigitToNumber converts the hex token token (i. e. token obeys 
 * (token >= '0' && token <= '9') || (token >= 'A' && token <= 'F')
 * ) into the corresponding number.
 * 
 * If token does not obey this condition 0xFF ((uint8_t) -1) is returned.
 */
uint8_t readHexDigit(char token);

#ifdef __cplusplus
}
#endif

#endif
