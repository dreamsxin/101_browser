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

#ifndef _Ogg_Skeleton_h
#define _Ogg_Skeleton_h

#include "MiniStdlib/cstdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

typedef struct
{
	char Identifier[8]; // 'fishead\0'
	uint16_t Version_major;
	uint16_t Version_minor;
	uint64_t Presentationtime_numerator;
	uint64_t Presentationtime_denominator;
	uint64_t Basetime_numerator;
	uint64_t Basetime_denominator;
	uint32_t UTC[5];
} FisheadIdentHeader;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif
