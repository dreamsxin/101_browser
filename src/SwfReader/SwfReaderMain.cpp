#include "BasicDataStructures/Stream/FileReadStream.h"
#include <cstdlib>
#include <cstring>

int main(int argc, char** argv)
{
	FileReadStream stream;

	if (!stream.open("testfiles/swf/watch-vfl127661.swf"))
		exit(1);

	char header[3];

	if (stream.read(header, 1, 3)!=3)
		exit(1);
	
	BasicReadStream* streamToContinue = NULL;

	bool compressed;

	if (strncmp(header, "CWS", 3))
		compressed = true;
	else if (strncmp(header, "FWS", 3))
		compressed = false;
	else
	{
		exit(1);
	}

	printf("Reading SWF succeeded\n");
	return 0;
}
