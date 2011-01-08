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

#include "TestSuite/TestAlgorithm.h"
#include <stdlib.h>
#include "MiniStdlib/cstdint.h"
#include "MiniStdlib/MTAx_cstdio.h"

void testAlgorithm_BitRead()
{
	// Source:
	// http://www.random.org/integers/?num=2&min=0&max=15&col=2&base=16&format=plain&rnd=new
	uint8_t bytes2[] = { 0xe4 };

	// Source:
	// http://www.random.org/integers/?num=20&min=0&max=15&col=2&base=16&format=plain&rnd=new
	uint8_t bytes10[] = {
		0x4c,
		0xd7,
		0x86,
		0x4a,
		0x74,
		0xa1,
		0x0c,
		0xa1,
		0x28,
		0xf8
	};
}
