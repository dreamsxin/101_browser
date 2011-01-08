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

#include "BasicDataStructures/Stream/FileReadStream.h"
#include <cstdlib>
#include <cstring>
#include <list>
using namespace std;

enum PdfReadState
{
	PdfReadStateDefault
};

bool readComment(FileReadStream* in_pStream)
{
	char datum;
	printf("COMMENT\n");

	while (true)
	{
		if (in_pStream->read(&datum, 1, 1) == 0)
			return false;

		if (datum == '\n')
			break;
		else if (datum == '\r')
		{
			if (in_pStream->read(&datum, 1, 1) == 0)
				return false;

			if (datum != '\n')
			{
				in_pStream->revert(1);
			}

			break;
		}
	}

	return true;
}

void readPdf(FileReadStream* in_pStream)
{
	PdfReadState state = PdfReadStateDefault;
	char datum;

	while (true)
	{
		if (in_pStream->read(&datum, 1, 1) == 0)
			return;

		switch (state)
		{
		case PdfReadStateDefault:
			{
				// % character begins comment
				if (datum == '%')
				{
					if (!readComment(in_pStream))
						return;
				}
				else
				{
					printf("%c", datum);
				}
			}
			break;
		}
	}
}

int main()
{
	FileReadStream stream;

	if (!stream.open("testfiles/pdf/swf_file_format_spec_v10.pdf"))
		exit(1);

	readPdf(&stream);

	return 0;
}
