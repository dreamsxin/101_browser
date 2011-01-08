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
#include "GIF/GIF.h"

void testGIF()
{
	// We want to be sure...
	test(sizeof(Logical_Screen_Descriptor) == 7);
	test(sizeof(Graphic_Control_Extension) == 5);
	test(sizeof(Image_Descriptor) == 10);

	{
		char* filenames[] = {
			"testfiles/gif/wikipedia/GifSampleSmall.gif",
			"testfiles/gif/wikipedia/GifSample.gif",
			/*
			* Provides test case for Application Extension block
			*/
			"testfiles/gif/wikipedia/Newtons_cradle_animation_book_2.gif",
			/*
			* Provides test case for Application Extension block
			*/
			"testfiles/gif/wikipedia/Rotating_earth_(large).gif",
			/*
			* Provides test case for Application Extension block
			*/
			"testfiles/gif/wikipedia/Rotating_earth_(small).gif",
			/*
			* Provides test case for Comment Extension block
			*/
			"testfiles/gif/wikipedia/Sunflower_as_gif_small.gif",

			"testfiles/gif/a_image.gif",
			"testfiles/gif/tc217.gif",

			"testfiles/gif/fileformat.info/GMARBLES.GIF",
			"testfiles/gif/fileformat.info/MARBLES.GIF",
			"testfiles/gif/fileformat.info/WFPC01.GIF",
			"testfiles/gif/fileformat.info/WFPC02.GIF",
			"testfiles/gif/fileformat.info/WFPC03.GIF",
			"testfiles/gif/fileformat.info/WFPC04.GIF",
			"testfiles/gif/fileformat.info/WFPC05.GIF",
			"testfiles/gif/fileformat.info/WFPC06.GIF"
		};

		FILE* imgFile;
		size_t filenameIdx;

		for (filenameIdx = 0; filenameIdx < sizeof(filenames)/sizeof(char*); filenameIdx++)
		{
			GIF_Data_Stream dataStream;
			ReadResult readResult;

			imgFile = MTAx_fopen(filenames[filenameIdx], "rb");

			test(imgFile != NULL);

			if (imgFile != NULL)
			{
				readResult = read_GIF_Data_Stream(imgFile, &dataStream);
				test(ReadResultOK == readResult);

				fclose(imgFile);
			}
		}
	}
}
