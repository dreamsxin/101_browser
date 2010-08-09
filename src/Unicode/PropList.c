#include "Unicode/PropList.h"

bool readPropList(FILE* in_file, char* in_property)
{
	unsigned char token;

	while (true)
	{
		if (fread(&token, 1, 1, in_file) != 1)
		{
			return true;
		}

		if (token == '#')
		{
			/* Read the line until its end */
			
		}
		else if (token == '\n')
		{
			continue;
		}
		else
		{

		}
	}

}