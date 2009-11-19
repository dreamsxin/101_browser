#include <cstdio>
#include <cstdlib>

#include "BasicDataStructures/Stream/FileReadStream.h"
#include "BasicDataStructures/Parser/StringParser.h"
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
	stringParser.open("2 Docume");

	char buffer[256];

	while (true)
	{
		size_t readCount = stream.read(buffer, 1, 256);
		if (readCount == 0)
			break;

		size_t writeCount = stringParser.write(buffer, 1, readCount);
		if (writeCount == 0)
			break;
	}

	XmlDefinition definition;
}
