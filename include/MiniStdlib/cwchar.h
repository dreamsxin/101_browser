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

#ifndef _MTAx_MiniStdlib_cwchar_h
#define _MTAx_MiniStdlib_cwchar_h

#ifdef __cplusplus
#include <cwchar>
#else
#include <wchar.h>
#endif

#ifndef _WIN32
/* we really need 2 macros here since - for example else _CRT_WIDE(__FILE__) will be expanded to L__FILE__ */
#define __CRT_WIDE(x) L ## x
#define _CRT_WIDE(x) __CRT_WIDE(x)
#endif

#endif