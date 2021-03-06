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

#include <stdio.h>
#include <stdlib.h>
#include "MiniStdlib/cstdint.h"
#include "TestSuite/TestSuite.h"

unsigned int testNr = 0;
uint8_t verbose = 0;
uint8_t exitOnFailure = 1;

void testPassed(const char *_Message, const char *_File, unsigned int _Line)
{
	if (verbose)
		printf("Test %u testing %s in %s:%u OK\n", testNr, _Message, _File, _Line);
	else
		printf("Test %u OK\n", testNr);

	testNr++;
}

void testFailed(const char *_Message, const char *_File, unsigned int _Line)
{
	printf("Test %u testing %s in %s:%u FAILED\n", testNr, _Message, _File, _Line);

	testNr++;

	if (exitOnFailure)
		exit(EXIT_FAILURE);
}
