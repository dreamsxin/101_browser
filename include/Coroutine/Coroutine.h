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

#ifndef _MTAx_Coroutine_Coroutine_h
#define _MTAx_Coroutine_Coroutine_h

#if defined(_WIN32)
# include "CoroutineWin/CoroutineWin.h"
#elif defined(__unix)
# include "CoroutinePosix/CoroutinePosix.h"
#else
# error Your OS seems not to be supported
#endif

#endif
