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

#ifndef _MTAx_HTML5_2_5_6_h
#define _MTAx_HTML5_2_5_6_h

#include "MiniStdlib/cstdint.h"
#include "Util/Iterator.h"
#include "Util/ReadResult.h"

/*
* http://www.whatwg.org/specs/web-apps/current-work/multipage/common-microsyntaxes.html#simple-color
* "A simple color consists of three 8-bit numbers in the range 0..255, 
* representing the red, green, and blue components of the color respectively, 
* in the sRGB color space. [SRGB]"
*/
typedef struct
{
	uint8_t red, green, blue;
} SimpleColor;

#ifdef _WIN32
	__declspec(dllexport)
#endif
ReadResult parseSimpleColor(SingleIterator in_iterator, void* in_iteratorState, 
SimpleColor* in_pSimpleColor);

#endif
