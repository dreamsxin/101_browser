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

#ifndef _MTAx_MiniStdlib_string_macros_h
#define _MTAx_MiniStdlib_string_macros_h

#define _TO_STRING(x) #x
#define TO_STRING(x) _TO_STRING(x)

#define _WIDEN(x) L ## x
#define WIDEN(x) _WIDEN(x)

#define _APPEND(x, y) x ## y
#define APPEND(x, y) _APPEND(x, y)

#endif
