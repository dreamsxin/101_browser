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
#include "MiniStdlib/memory.h"

void test_MiniStdlib()
{
	// Raw numbers from http://www.random.org/cgi-bin/randbyte?nbytes=24&format=h
	
	int arr0[] = {
		0xd4b3dde1, 
		0xca027453, 
		0x3f5685b3
	};
	int arr1[] = {
		0x08b6878d, 
		0xba63f3e9, 
		0x968fb1e5 
	};

	memxchg(arr0, arr1, sizeof(arr0));

	test(0x08b6878d == arr0[0]);
	test(0xba63f3e9 == arr0[1]);
	test(0x968fb1e5 == arr0[2]);

	test(0xd4b3dde1 == arr1[0]);
	test(0xca027453 == arr1[1]);
	test(0x3f5685b3 == arr1[2]);
}
