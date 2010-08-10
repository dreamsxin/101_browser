#include "Unicode/PropList.h"
#include "Util/Interval.hpp"
#include "Util/Unicode.h"
#include <list>
#include <stdexcept>
using namespace std;

/*!
 * Precondition: (token >= '0' && token <= '9') || (token >= 'A' && token <= 'F')
 */
uint8_t readHexDigit(char token)
{
	if (token >= '0' && token <= '9')
	{
		return (token-'0');
	}
	else
	{
		assert(token >= 'A' && token <= 'F');

		return (token - 'A');
	}
}

bool readPropList(FILE* in_file, char* in_property, void* out_ppIntervals)
{
	Interval<UnicodeCodePoint>** ppIntervals = static_cast<Interval<UnicodeCodePoint>**>(out_ppIntervals);
	std::list<Interval<UnicodeCodePoint> > listIntervals;

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
			if ((token >= '0' && token <= '9') || (token >= 'A' && token <= 'F'))
			{
				UnicodeCodePoint currentCodePoint = readHexDigit(token);
				UnicodeCodePoint nextCodePoint;

				while (true)
				{
					if (fread(&token, 1, 1, in_file) != 1)
					{
						return false;
					}

					if (token >= '0' && token <= '9' || (token >= 'A' && token <= 'F'))
					{
						currentCodePoint = 16*currentCodePoint + readHexDigit(token);
					}
					else if (token == '.')
					{
						if (fread(&token, 1, 1, in_file) != 1)
						{
							return false;
						}

						if (token != '.')
							return false;


					}
					else if (token == ' ')
					{
						nextCodePoint = currentCodePoint;
						break;
					}
					else
						return false;
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