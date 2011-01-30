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

#ifndef _MTAx_MiniStdlib_cstdbool_h
#define _MTAx_MiniStdlib_cstdbool_h

#if defined(_MSC_VER)
# ifndef __cplusplus

#  include "MiniStdlib/cstdint.h"

typedef uint8_t _Bool;

#define bool _Bool
#define true 1
#define false 0
#define __bool_true_false_are_defined 1

# endif
#else
# ifndef __cplusplus
#  include <stdbool.h>
# else
static_assert(sizeof(bool) == 1, "sizeof(bool) (C++) must be 1");
# endif
#endif

#endif
