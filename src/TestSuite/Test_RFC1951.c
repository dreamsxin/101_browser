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

#include "RFC1951/RFC1951.h"
#include "IO/FileByteStream.h"

void test_RFC1951()
{
	FileByteStreamState fileByteStreamState;
	bool result = fileByteReadStreamState_create("testfiles/deflate/zeros.raw", &fileByteStreamState);

	test(result);

	if (result)
	{
		test(ReadResultOK == parseRFC1951(cFileByteStreamInterface, &fileByteStreamState));

		fileByteReadStreamState_destroy(&fileByteStreamState);
	}
}
