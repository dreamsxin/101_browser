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
#include "Ogg/Ogg.h"
#include "Ogg/Skeleton.h"
#include "IO/FileByteStream.h"

static const char *ogg_filenames[] = {
	"testfiles/video/big_buck_bunny.ogv",
	"testfiles/video/fridge.ogv",
	"testfiles/video/garden1.ogv",
	"testfiles/video/garden2.ogv",
	"testfiles/video/VfE.ogv",
	"testfiles/video/windowsill.ogv"
};

void test_Ogg()
{
	FileByteStreamState fileByteStreamState;
	bool result;
	int idx;

	test(64 == sizeof(FisheadIdentHeader30));
	test(80 == sizeof(FisheadIdentHeader40));

	for (idx = 0; idx < sizeof(ogg_filenames)/sizeof(char*); idx++)
	{
		printf("Testing file %s\n", ogg_filenames[idx]);

		result = fileByteReadStreamStateInit(ogg_filenames[idx], 
			&fileByteStreamState);

		test(result);

		if (result)
		{
			ReadResult readResult = readOgg(&fileByteStreamState, getFileByteStreamInterface());
			test(ReadResultOK == readResult);
			fileByteReadStreamStateDestroy(&fileByteStreamState);
		}
	}
}