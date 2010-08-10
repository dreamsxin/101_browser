#include "Unicode/PropList.h"
#include "Util/Interval.hpp"
#include "Util/Unicode.h"

bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals)
{
	Interval<UnicodeCodePoint>** ppIntervals = static_cast<Interval<UnicodeCodePoint>**>(out_ppIntervals);

	unsigned char token;

	while (true)
	{
		if (fread(&token, 1, 1, in_file) != 1)
		{
			break;
		}

		if (token == '#')
		{
			// Read until the end of the line
			while (token != '\n')
			{
				if (fread(&token, 1, 1, in_file) != 1)
				{
					return false;
				}
			}
		}
		else if (token == '\n')
		{
			continue;
		}
		else
		{
			if (token >= '0' && token <= '9')
			{
				UnicodeCodePoint currentCodePoint = token-'0';
				UnicodeCodePoint nextCodePoint;

				while (true)
				{
					if (fread(&token, 1, 1, in_file) != 1)
					{
						return false;
					}

					if (token >= '0' && token <= '9')
					{
						currentCodePoint = 10*currentCodePoint+(token-'0');
					}
					else if (token == '.')
					{

					}
					else if (token == ' ')
					{
						nextCodePoint = currentCodePoint;
						break;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}