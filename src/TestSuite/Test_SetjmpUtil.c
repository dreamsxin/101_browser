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

#include "TestSuite/Tests.h"
#include "TestSuite/TestSuite.h"
#include "SetjmpUtil/ConditionalLongjmp.h"
#include <stdio.h>

void testSetjmpUtil0()
{
	jmp_buf jmpBuf0;
	int result;
	unsigned counter = 0;

	// the = is correct
	if (result = setjmp(jmpBuf0))
	{
		test(21 == result);
		test(counter == 1);
		counter = 2;
		return;
	}

	test(0 == counter);
	counter = 1;

	longjmp(jmpBuf0, 21);
}

void testSetjmpUtil1()
{
	jmp_buf jmpBuf0, jmpBuf1;
	int result;
	unsigned counter = 0;

	// the = is correct
	if (result = setjmp(jmpBuf0))
	{
		test(42 == result);
		test(counter == 3);
		counter = 4;
		return;
	}

	test(0 == counter);
	counter = 1;

	// the = is correct
	if (result = XCHG_AND_SETJMP(jmpBuf0, jmpBuf1))
	{
		test(21 == result);
		test(counter == 2);
		counter = 3;
		xchgAndLongjmp(jmpBuf1, jmpBuf0, 42);
	}

	test(1 == counter);
	counter = 2;

	longjmp(jmpBuf0, 21);
}

void testSetjmpUtil2()
{
	jmp_buf jmpBuf0, jmpBuf1, jmpBuf2;
	int result;
	unsigned counter = 0;

	// the = is correct
	if (result = setjmp(jmpBuf0))
	{
		test(63 == result);
		test(counter == 5);
		counter = 6;
		return;
	}

	test(0 == counter);
	counter = 1;

	// the = is correct
	if (result = XCHG_AND_SETJMP(jmpBuf0, jmpBuf1))
	{
		test(42 == result);
		test(counter == 4);
		counter = 5;
		xchgAndLongjmp(jmpBuf1, jmpBuf0, 63);
	}

	test(1 == counter);
	counter = 2;

	// the = is correct
	if (result = XCHG_AND_SETJMP(jmpBuf0, jmpBuf2))
	{
		test(21 == result);
		test(counter == 3);
		counter = 4;
		xchgAndLongjmp(jmpBuf2, jmpBuf0, 42);
	}

	test(2 == counter);
	counter = 3;

	longjmp(jmpBuf0, 21);
}

void testSetjmpUtil()
{
	printf("Calling testSetjmpUtil0\n");
	testSetjmpUtil0();
	printf("Calling testSetjmpUtil1\n");
	testSetjmpUtil1();
	printf("Calling testSetjmpUtil2\n");
	testSetjmpUtil2();
}
