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

#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "RFC1950/RFC1950_adler32.h"
#include "RFC1950/RFC1950_zlib.h"
#include <string.h> // for strlen

void test_RFC1950_adler32()
{
	/*
	 * Test case taken from
	 * http://en.wikipedia.org/w/index.php?title=Adler-32&oldid=366924289#Example
	 */
	char wikipediaString[] = "Wikipedia";
	uint32_t wikipediaStringAdler = RFC1950_adler32_compute((uint8_t*) wikipediaString, strlen(wikipediaString));
	test(wikipediaStringAdler == 0x11E60398ul);
}

void test_RFC1950_zlib()
{
	test(sizeof(ZlibStreamHeader) == 2);
}

void test_RFC1950()
{
	printf("Testing RFC 1950 - adler32\n");
	test_RFC1950_adler32();
	printf("Testing RFC 1950 - zlib\n");
	test_RFC1950_zlib();
}
