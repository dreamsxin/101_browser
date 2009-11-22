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
		fprintf(stderr, "Usage: Parser filename\n");
		exit(1);
	}

	FileReadStream stream;

	if (!stream.open(argv[argc-1]))
	{
		fprintf(stderr, "Could not open file %s\n", argv[argc-1]);
		exit(1);
	}

	StringParser stringParser;
	UntilCharacterParser untilCharacterParser;

	stringParser.open("2 Docume");

	char currentToken;

	untilCharacterParser.open('\n');

	while (true)
	{
		size_t readCount = stream.read(&currentToken, 1, 1);
		if (readCount == 0)
			break;

		bool parseOK = untilCharacterParser.parseToken(currentToken);

		if (!untilCharacterParser.isOpen())
		{
			if (untilCharacterParser.mReadCharacters.size()>0 && *untilCharacterParser.mReadCharacters.begin()=='[')
			{
				for (std::list<char>::iterator i=untilCharacterParser.mReadCharacters.begin(); 
					i!=untilCharacterParser.mReadCharacters.end(); i++)
				{
					printf("%c", *i);
				}

				printf("\n");
			}

			untilCharacterParser.open('\n');
		}
	}

	XmlDefinition definition;
}
