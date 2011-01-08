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

#include "RFC1950/RFC1950_adler32.h"
#include "Template/HigherOrderFunctions/Fold.h"

uint32_t RFC1950_adler32_foldl(uint32_t in_adler, uint8_t *in_buffer, size_t in_bufferSize)
{
	return MTAx::HigherOrderFunctions::foldl<uint32_t, uint8_t>(&RFC1950_adler32_update, 
		in_adler, in_buffer, in_bufferSize);
}
