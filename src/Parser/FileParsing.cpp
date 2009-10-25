#include "Parser/FileParsing.h"

bool parseFile(FILE* in_file, AbstractParser* in_parser)
{
	in_parser->initialize();

	while (true)
	{
		char actChar;

		if (fread(&actChar, 1, 1, in_file)==1)
		{
			if (!in_parser->isFinished())
			{
				in_parser->parseToken(actChar);
			}
			else
			{
				return false;
			}
		}
		else
		{
			return in_parser->acceptWhenTerminate();
		}
	}
}
