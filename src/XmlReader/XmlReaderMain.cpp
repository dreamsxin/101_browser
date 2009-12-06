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
