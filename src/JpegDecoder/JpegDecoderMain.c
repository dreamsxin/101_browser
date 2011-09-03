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

#include "JpegDecoder/JpegDecoder_SectionE.h"
#include "IO/FileByteStream.h"
#include <stdlib.h>
#include <assert.h>


int main()
{
	char* filenames[] = {
		// JPEG Baseline 420/411 EXIF Image
		"testfiles/jpeg/xhtml-print/jpeg420exif.jpg",
		// JPEG Baseline 422 JFIF Image
		"testfiles/jpeg/xhtml-print/jpeg422jfif.jpg",
		// JPEG Baseline 400 JFIF Image
		"testfiles/jpeg/xhtml-print/jpeg400jfif.jpg",
		// JPEG Baseline 444 Image; Image With All Possible APPx Markers
		"testfiles/jpeg/xhtml-print/jpeg444.jpg",
		
		"testfiles/T083v1_0/T83_process1/A1.jpg"
		//"testfiles/T083v1_0/T83_process1/B1.jpg";
		//"testfiles/T083v1_0/T83_process1/B2.jpg";

		//"testfiles/T083v1_0/T83_process2/A1.jpg";
		//"testfiles/T083v1_0/T83_process2/B1.jpg";
		//"testfiles/T083v1_0/T83_process2/B2.jpg";
		//"testfiles/T083v1_0/T83_process2/C1.jpg";
		//"testfiles/T083v1_0/T83_process2/C2.jpg";

		//"testfiles/T083v1_0/T83_process3/A1.jpg";
		//"testfiles/T083v1_0/T83_process3/B1.jpg";
		//"testfiles/T083v1_0/T83_process3/B2.jpg";
		//"testfiles/T083v1_0/T83_process3/D1.jpg";
		//"testfiles/T083v1_0/T83_process3/D2.jpg";

		//"testfiles/T083v1_0/T83_process4/A1.jpg";
		//"testfiles/T083v1_0/T83_process4/B1.jpg";
		//"testfiles/T083v1_0/T83_process4/B2.jpg";
		//"testfiles/T083v1_0/T83_process4/C1.jpg";
		//"testfiles/T083v1_0/T83_process4/C2.jpg";
		//"testfiles/T083v1_0/T83_process4/E1.jpg";
		//"testfiles/T083v1_0/T83_process4/E2.jpg";

		//"testfiles/T083v1_0/T83_process5/A1.jpg";
		//"testfiles/T083v1_0/T83_process5/B1.jpg";
		//"testfiles/T083v1_0/T83_process5/B2.jpg";
		//"testfiles/T083v1_0/T83_process5/D1.jpg";
		//"testfiles/T083v1_0/T83_process5/D2.jpg";
		//"testfiles/T083v1_0/T83_process5/F1.jpg";
		//"testfiles/T083v1_0/T83_process5/F2.jpg";

		//"testfiles/T083v1_0/T83_process6/A1.jpg";
		//"testfiles/T083v1_0/T83_process6/B1.jpg";
		//"testfiles/T083v1_0/T83_process6/B2.jpg";
		//"testfiles/T083v1_0/T83_process6/C1.jpg";
		//"testfiles/T083v1_0/T83_process6/C2.jpg";
		//"testfiles/T083v1_0/T83_process6/G1.jpg";

		//"testfiles/T083v1_0/T83_process7/A1.jpg";
		//"testfiles/T083v1_0/T83_process7/B1.jpg";
		//"testfiles/T083v1_0/T83_process7/B2.jpg";
		//"testfiles/T083v1_0/T83_process7/D1.jpg";
		//"testfiles/T083v1_0/T83_process7/D2.jpg";
		//"testfiles/T083v1_0/T83_process7/H1.jpg";

		//"testfiles/T083v1_0/T83_process9/A1.jpg";
		//"testfiles/T083v1_0/T83_process9/B1.jpg";
		//"testfiles/T083v1_0/T83_process9/B2.jpg";
		//"testfiles/T083v1_0/T83_process9/D1.jpg";
		//"testfiles/T083v1_0/T83_process9/D2.jpg";
		//"testfiles/T083v1_0/T83_process9/F1.jpg";
		//"testfiles/T083v1_0/T83_process9/F2.jpg";
		//"testfiles/T083v1_0/T83_process9/H1.jpg";
		//"testfiles/T083v1_0/T83_process9/J1.jpg";

		//"testfiles/T083v1_0/T83_process10/A1.jpg";
		//"testfiles/T083v1_0/T83_process10/B1.jpg";
		//"testfiles/T083v1_0/T83_process10/B2.jpg";
		//"testfiles/T083v1_0/T83_process10/C1.jpg";
		//"testfiles/T083v1_0/T83_process10/C2.jpg";
		//"testfiles/T083v1_0/T83_process10/G1.jpg";
		//"testfiles/T083v1_0/T83_process10/K1.jpg";

		//"testfiles/T083v1_0/T83_process11/A1.jpg";
		//"testfiles/T083v1_0/T83_process11/B1.jpg";
		//"testfiles/T083v1_0/T83_process11/B2.jpg";
		//"testfiles/T083v1_0/T83_process11/D1.jpg";
		//"testfiles/T083v1_0/T83_process11/D2.jpg";
		//"testfiles/T083v1_0/T83_process11/H1.jpg";
		//"testfiles/T083v1_0/T83_process11/L1.jpg";

		//"testfiles/T083v1_0/T83_process13/A1.jpg";
		//"testfiles/T083v1_0/T83_process13/B1.jpg";
		//"testfiles/T083v1_0/T83_process13/B2.jpg";
		//"testfiles/T083v1_0/T83_process13/D1.jpg";
		//"testfiles/T083v1_0/T83_process13/D2.jpg";
		//"testfiles/T083v1_0/T83_process13/F1.jpg";
		//"testfiles/T083v1_0/T83_process13/F2.jpg";
		//"testfiles/T083v1_0/T83_process13/H1.jpg";
		//"testfiles/T083v1_0/T83_process13/J1.jpg";
		//"testfiles/T083v1_0/T83_process13/L1.jpg";
		//"testfiles/T083v1_0/T83_process13/N1.jpg";

		//"testfiles/T083v1_0/T83_process14/O1.jpg";
		//"testfiles/T083v1_0/T83_process14/O2.jpg";

		//"testfiles/T083v1_0/T83_process15/P1.jpg";
		//"testfiles/T083v1_0/T83_process15/P2.jpg";


		//"testfiles/T087v1_0/T87_test-1-2-3-4-5-6/T8C0E0.JLS";
		//"testfiles/T087v1_0/T87_test-1-2-3-4-5-6/T8C0E3.JLS";
		};

	int idx;

	for (idx = 0; idx < sizeof(filenames)/sizeof(filenames[0]); idx++)
	{
		FileByteStreamState fileByteStreamState;
		ReadResult readResult;

		printf("Opening file %s\n", filenames[idx]);

		if (!fileByteReadStreamStateInit(filenames[idx], &fileByteStreamState))
		{
			fprintf(stderr, "Could not open file %s\n", filenames[idx]);
			exit(1);
		}

		if ((readResult = Decode_image(&fileByteStreamState, getFileByteStreamInterface())) != ReadResultOK)
		{
			fprintf(stderr, "Error when reading file: %s; error code: %u\n", filenames[idx], readResult);
		}

		fileByteReadStreamStateDestroy(&fileByteStreamState);

		printf("\n\n");
	}

	
}
