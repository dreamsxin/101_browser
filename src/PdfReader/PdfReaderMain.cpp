#include "BasicDataStructures/Stream/FileReadStream.h"
#include <cstdlib>
#include <cstring>
#include <vector>
using namespace std;

/*!
 * Clears the passed vector until we get a \r, \n or \r\n and returns
 * the line without the ending
 *
 * Return value: true if the was no EOF before the \r, \n, \r\n
 *               false otherwise
 */
bool readLine(RevertibleReadStream* in_stream, vector<char>* in_readData)
{
	char datum;

	in_readData->clear();

	while (true)
	{
		size_t readBytes = in_stream->read(&datum, 1, 1);

		if (readBytes == 1)
		{
			if (datum == '\n')
				return true;
			else if (datum=='\r')
			{
				readBytes = in_stream->read(&datum, 1, 1);
				if (readBytes == 1)
				{
					if (datum != '\n')
					{
						in_stream->revert(1);
					}
				}

				return true;
			}
			else
			{
				in_readData->push_back(datum);
			}
		}
		else
		{
			return false;
		}
	}
}

int main()
{
	FileReadStream stream;

	if (!stream.open("testfiles/pdf/swf_file_format_spec_v10.pdf"))
		exit(1);

	vector<char> currentLine;

	while (readLine(&stream, &currentLine))
	{
		for (vector<char>::iterator i=currentLine.begin(); i!=currentLine.end(); i++)
		{
			printf("%c", *i);
		}

		printf("\n");
	}
	
	return 0;
}
