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

#ifndef _MTAx_MiniStdlib_cstdint_h
#define _MTAx_MiniStdlib_cstdint_h

# ifdef __cplusplus
/*
 * Visual Studio 2010 is the first version to 
 * contain cstdint. In the versions before we
 * have to use boost. Also gcc does not like this
 */
#  if defined(_MSC_VER) && (_MSC_VER < 1600)
#include "contrib/pstdint.h"
#  elif defined(__GNUC__)
#include <stdint.h>
#  else
#include <cstdint>
#  endif
# else
#  if defined(_MSC_VER) && (_MSC_VER < 1600)
#include "contrib/pstdint.h"
#  else
#include <stdint.h>
#  endif
# endif

#endif
