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

#include <cstdio>
#include <cstdlib>

#include "BasicDataStructures/Stream/FileReadStream.h"
#include "BasicDataStructures/Parser/StringParser.h"
#include "BasicDataStructures/Parser/UntilCharacterParser.h"
#include "XmlReader/XmlDefinition.h"

int main(int argc, char** argv)
{
	if (argc<2)
	{
		fprintf(stderr, "Usage: XmlReader filename\n");
		exit(1);
	}

	FileReadStream stream;

	if (!stream.open(argv[argc-1]))
	{
		fprintf(stderr, "Could not open file %s\n", argv[argc-1]);
		exit(1);
	}

	StringParser stringParser("2 Docume");
	UntilCharacterParser untilCharacterParser('\n');

	while (true)
	{
		char currentToken;

		if (stream.read(&currentToken, 1, 1) != 1)
			break;

		bool parseOK = untilCharacterParser.parseToken(currentToken);

		if (untilCharacterParser.isDone())
		{
			if (untilCharacterParser.mReadCharacters.size()>0 && 
				(*untilCharacterParser.mReadCharacters.begin()=='[' || *untilCharacterParser.mReadCharacters.begin()=='\t'))
			{
				for (std::list<char>::iterator i=untilCharacterParser.mReadCharacters.begin(); 
					i!=untilCharacterParser.mReadCharacters.end(); i++)
				{
					printf("%c", *i);
				}

				printf("\n");
			}

			untilCharacterParser.reset();
		}
	}

	XmlDefinition definition;
}
